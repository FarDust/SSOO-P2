#pragma once

#include <stdlib.h>
#include "../spells/spell.h"
#include "../entity/entity.h"

#define PLAYER_NUMBER 5
#define CLASS_NUMBER 3
typedef enum
{
  Hunter,
  Medic,
  Hacker,
} PlayerClass;

typedef struct player
{
  char* name;
  Entity *properties;
  PlayerClass spec;
  Spell current_spell;
} Player;

Player ** get_player_list();
size_t get_player_count();
Player * spawn_player();
size_t get_spec_health(PlayerClass spec);
void set_player_class(Player * player, PlayerClass spec);
char * get_class_name(PlayerClass spec);
void kill_player(Player * player);
void show_spells(Player *player);
Spell get_spell_slot(PlayerClass spec, Slot slot);
void select_spell(Player *player, Slot spell);
char* cast_spell(Entity *caster, Entity * target, Spell spell);
