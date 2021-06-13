#include <stdlib.h>
#include "../spells/spell.h"
#include "../player/player.h"

typedef enum
{
	GreatJagRuz,
	Ruzalos,
	Ruiz,
} MonsterClass;

typedef struct monster
{
  Entity *properties;
  MonsterClass spec;
  Spell current_spell;
} Monster;

void select_spell(Spell spell);
void cast_monster_spell(Monster *monster, Entity * target);
