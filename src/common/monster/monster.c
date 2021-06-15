#include "./monster.h"
#include <time.h>

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
    break;
  case Ruzalos:
    monster->properties->health = 20000;
    break;
  case Ruiz:
    monster->properties->health = 25000;
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
    if (r < 40)
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

void cast_monster_spell(Monster *monster, Player** players, int n_players, int rounds)
{
  time_t t;
  srand((unsigned) time(&t));
  Entity* targets[n_players];
  for (size_t i = 0; i < n_players; i++)
  {
    targets[i] = players[i]->properties;
  }
  int target = rand() % n_players;

  switch (monster->current_spell)
  {
  case Salto:
    salto(monster->properties, targets[target]);
    break;
  case EspinaVenenosa:
    espina_venenosa(targets[target]);
    break;
  case Ruzgar:
    ruzgar(targets[target]);
    break;
  case Coletazo:
    coletazo(targets, n_players);
    break;
  case CasoDeCopia:
    caso_de_copia(monster->properties, players[target]->spec, targets[target]);
    break;
  case Reprobatron:
    reprobatron(targets[target]);
    break;
  case SudoRmRf:
    sudoRmRf(rounds, targets, n_players);
    break;

  default:
    break;
  }
}