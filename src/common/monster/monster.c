#include "./monster.h"
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

Monster* MONSTER;

Monster * spawn_monster(MonsterClass spec){
  Monster *monster = malloc(sizeof(Monster));
  monster->properties = spawn_entity();
  monster->name = spec;
  monster->current_spell = 0;
  
  switch (monster->name)
  {
  case GreatJagRuz:
    monster->properties->health = 10000;
    monster->properties->name = "Great JagRuz";
    break;
  case Ruzalos:
    monster->properties->health = 20000;
    monster->properties->name = "Ruzalos";
    break;
  case Ruiz:
    monster->properties->health = 25000;
    monster->properties->name = "Ruiz";
    break;

  default:
    break;
  }
  monster->properties->max_health = monster->properties->health;
  MONSTER = monster;
  return monster;
}

void select_monster_spell(Monster* monster)
{
  time_t t;
  srand((unsigned) time(&t));
  int r = rand()%100;
  switch (monster->name)
  {
  case GreatJagRuz:
    if (r < 50 )
    {
      monster->current_spell = Ruzgar;
    } else {
      monster->current_spell = Coletazo;
    }
    
    break;
  case Ruzalos:
    if (monster->properties->buff[JumpBlocked]==1)
    {
      monster->current_spell = EspinaVenenosa;
      monster->properties->buff[JumpBlocked]=0;
      return;
    }
    if (r < 0)
    {
      monster->current_spell = Salto;
    } else {
      monster->current_spell = EspinaVenenosa;
    }
    break;
  case Ruiz:
    if (r < 40)
    {
      monster->current_spell = CasoDeCopia;
    } else if (r < 60)
    {
      monster->current_spell = Reprobatron;
    } else {
      monster->current_spell = SudoRmRf;
    }
    break;

  default:
    break;
  }
}

void kill_monster(Monster* monster){
  free(monster->properties);
  free(monster);
}

const char* cast_monster_spell(Monster *monster, Player** players, int n_players, int rounds)
{
  char* msg;
  msg = (char *)calloc(200, 1);
  time_t t;
  srand((unsigned) time(&t));
  
  int posible_target_count = 0;
  for (size_t i = 0; i < n_players; i++)
  {
    if (players[i]->properties->health > 0){
      posible_target_count += 1;
    }
  }
  Entity* targets[posible_target_count];
  posible_target_count = 0;
  for (size_t i = 0; i < n_players; i++)
  {
    if (players[i]->properties->health > 0)
    {  
      targets[posible_target_count] = players[i]->properties;
      posible_target_count += 1;
    }
  }
  int target;
  if (monster->properties->buff[Taunted]){
    target = monster->properties->buff[TauntedBy];
  }else{
    target = rand() % posible_target_count;
  }

  printf("Entity %ld cast %s in Entity %ld\n", monster->properties->uuid, get_spell_name(monster->current_spell), targets[target]->uuid);
  char* response;
  switch (monster->current_spell)
  {
  case Salto:
    response = salto(monster->properties, targets[target]);
    write_message(msg, response);
    free(response);
    break;
  case EspinaVenenosa:
    response = espina_venenosa(monster->properties, targets[target]);
    write_message(msg, response);
    free(response);
    break;
  case Ruzgar:
    response = ruzgar(monster->properties, targets[target]);
    write_message(msg, response);
    free(response);
    break;
  case Coletazo:
    response = coletazo(monster->properties, targets, posible_target_count);
    write_message(msg, response);
    free(response);
    break;
  case CasoDeCopia:
    response = caso_de_copia(monster->properties, players[target]->spec, players[rand() % posible_target_count],targets[target]);
    write_message(msg, response);
    free(response);
    break;
  case Reprobatron:
    response = reprobatron(monster->properties, targets[target]);
    write_message(msg, response);
    free(response);
    break;
  case SudoRmRf:
    response = sudoRmRf(monster->properties, rounds, targets, posible_target_count);
    write_message(msg, response);
    free(response);
    break;

  default:
    break;
  }
  return msg;
}

