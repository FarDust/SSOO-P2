#include <stdlib.h>
#include "../spells/spell.h"
#include "../player/player.h"
#include "../spells/rulazos/rulazos_spells.h"
#include "../spells/ruiz/ruiz_spells.h"
#include "../spells/great_jagruz/great_jagruz_spells.h"

#define MAX_MONSTER_CLASSES 3

typedef enum
{
	GreatJagRuz,
	Ruzalos,
	Ruiz,
} MonsterClass;

typedef struct monster
{
  Entity *properties;
  MonsterClass name;
  Spell current_spell;
} Monster;


Monster * spawn_monster(MonsterClass spec);
Monster * set_monster(Monster *monster, MonsterClass spec);
void kill_monster(Monster* monster);
void select_monster_spell(Monster* monster);
MonsterClass get_random_monster();
void reset_monster();
const char* cast_monster_spell(Monster *monster, Player** players, int n_players, int rounds);
