#pragma once

#include <stdlib.h>
#include "../spells/spell.h"
#include "../entity/entity.h"

#define PLAYER_NUMBER 4
typedef enum
{
  Hunter,
  Medic,
  Hacker,
} PlayerClass;

typedef struct player
{
  Entity *properties;
  PlayerClass spec;
  Spell current_spell;
} Player;

Player * spawn_player(PlayerClass spec);
void kill_player(Player * player);
void show_spells(Player *player);
Spell get_spell_slot(PlayerClass spec, Slot slot);
void select_spell(Player *player, Slot spell);
void cast_spell(Entity *caster, Entity * target, Spell spell);
