#pragma once

#include <stdlib.h>
#include "../spells/spell.h"

typedef enum
{

} MonsterClass;

typedef struct monster
{
  size_t uuid;
  size_t health;
  MonsterClass spec;
  Spell current_spell;
} Monster;