#include "player.h"
#include "../spells/spec_mappers/spec_spells.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

Player *PLAYERS[PLAYER_NUMBER];
size_t current_player = 0;

Player * spawn_player(PlayerClass spec){
  Player *player = malloc(sizeof(Player));
  player->properties = spawn_entity();
  player->spec = spec;
  player->current_spell = 0;
  
  switch (player->spec)
  {
  case Hunter:
    player->properties->health = 5000;
    break;
  case Medic:
    player->properties->health = 3000;
    break;
  case Hacker:
    player->properties->health = 2500;
    break;

  default:
    break;
  }
  player->properties->max_health = player->properties->health;
  PLAYERS[current_player] = player;
  current_player += 1;
  return player;
}

void kill_player(Player * player){
  free(player->properties);
  free(player);
}

Spell get_spell_slot(PlayerClass spec, Slot slot){
  switch (spec)
  {
  case Hunter:
    return get_hunter_spell(slot);
    break;
  case Medic:
    return get_medic_spell(slot);
    break;
  case Hacker:
    return get_hacker_spell(slot);
    break;

  default:
    break;
  }
  return 0;
}

void select_spell(Player *player, Slot spell){
  player->current_spell = get_spell_slot(player->spec, spell);
}

void show_spells(Player *player){

  for (size_t slot = 0; slot < 3; slot++)
  {
    printf("%ld - %s\n", slot, get_spell_name(get_spell_slot(player->spec, slot)));
  }

}

void cast_spell(Entity *caster, Entity * target, Spell spell){
  printf("Entity %ld cast %s in Entity %ld\n", caster->uuid, get_spell_name(spell), target->uuid);
  switch (spell)
  {
  case Estocada:
    estocada(caster, target);
    break;
  case CorteCruzado:
    corte_cruzado(caster, target);
    break;
  case Distraer:
    distraer(caster, target);
    break;
  case Curar:
    curar(caster, target);
    break;
  case DestelloRegenerador:;
    size_t damage_dealt = destello_regenerador(caster, target);

    time_t t;
    srand((unsigned) time(&t));

    size_t selected_player = rand() % current_player;
    Player * new_target = PLAYERS[selected_player];
    destello_regenerador_side_effect(caster, new_target->properties, (size_t)round((double)damage_dealt / 2));
    break;
  case DescargaVital:
    descarga_vital(caster, target);
    break;
  case InyeccionSQL:
    inyeccion_sql(caster, target);
    break;
  case AtaqueDDOS:
    ataque_ddos(caster, target);
    break;
  case FuerzaBruta:
    fuerza_bruta(caster, target);
    break;
  
  default:
    break;
  }
}