#include "player.h"
#include "../spells/spec_mappers/spec_spells.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

Player *PLAYERS[PLAYER_NUMBER];
size_t current_player = 0;

char *CLASS_NAME[] = {
    "Cazador 🦌",
    "Medico 💊",
    "Hacker 💻"};

Player ** get_player_list(){
  return PLAYERS;
}

size_t get_player_count(){
  return current_player;
}

void reset_players(){
  for (size_t i = 0; i < get_player_count(); i++)
  {
    kill_player(PLAYERS[i]);
  }
  current_player = 0;
}

Player * spawn_player(){
  Player *player = malloc(sizeof(Player));
  player->index = current_player;
  player->properties = spawn_entity();
  player->current_spell = 0;
  player->spec = -1;
  PLAYERS[player->index] = player;
  current_player += 1;
  return player;
}

size_t get_spec_health(PlayerClass spec){
  size_t health = 0;
  switch (spec)
  {
  case Hunter:
    health = 5000;
    break;
  case Medic:
    health = 3000;
    break;
  case Hacker:
    health = 2500;
    break;

  default:
    break;
  }
  return health;
}

void set_player_class(Player * player, PlayerClass spec){
  player->spec = spec;
  player->properties->health = get_spec_health(spec);
  player->properties->max_health = player->properties->health;
}

char* get_class_name(PlayerClass spec){
  if (spec == -1){
    return "NULL";
  }
  return CLASS_NAME[spec];
}

void kill_player(Player * player){
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

char* cast_spell(Entity *caster, Entity * target, Spell spell){
  printf("%s cast %s in %s\n", caster->name, get_spell_name(spell), target->name);
  char* str;
  switch (spell)
  {
  case Estocada:
    return (char *)estocada(caster, target);
  case CorteCruzado:
    return (char *)corte_cruzado(caster, target);
  case Distraer:
    return (char *)distraer(caster, target);
  case Curar:
    return (char *)curar(caster, target);
  case DestelloRegenerador:;
    size_t damage_dealt = destello_regenerador(caster, target);

    time_t t;
    srand((unsigned) time(&t));

    size_t selected_player = rand() % current_player;
    Player * new_target = PLAYERS[selected_player];
    char* dr_msg = (char *)destello_regenerador_side_effect(caster, new_target->properties, (size_t)round((double)damage_dealt / 2));
    char msg[200];
    str = (char *)calloc(200, 1);
    sprintf(msg, "%s ha hecho %li de daño a %s\n", dr_msg, damage_dealt, target->name);
    free(dr_msg);
    write_message(str, msg);
    return str;
  case DescargaVital:
    return (char *)descarga_vital(caster, target);
  case InyeccionSQL:
    return (char *)inyeccion_sql(caster, target);
  case AtaqueDDOS:
    return (char *)ataque_ddos(caster, target);
  case FuerzaBruta:
    return (char *)fuerza_bruta(caster, target);
  
  default:
    return "\n";
  }
  return "\n";
}