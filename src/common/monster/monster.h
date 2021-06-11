#include <stdlib.h>
#include "../spells/spell.h"
#include "../players/player.h"

typedef enum
{
	GreatJagRuz,
	Ruzalos,
	Ruiz,
} MonsterClass;

typedef struct monster
{
	bool monster;
	char* name;
  size_t uuid;
  size_t health;
  int* aviable_spells;
  int fuerza_bruta_count;
  int sangrado;
  bool can_jump;
  MonsterClass spec;
  Spell current_spell;
} Monster;

// GreatJagRuz
void ruzgar(Monster* monster, Player* target); // 50
void coletazo(Monster* monster, Player** targets, int targets_n); // 50
// Ruzalos
void salto(Monster* monster, Player* target); // 40
void espina_venenosa(Monster* monster, Player* target); // 60
// Ruiz
void caso_de_copia(Monster* monster, Player* player_to_steal, Player* target); // 40
void reprobatron(Monster* monster, Player* target); // 20
voi sudoRmRf(Monster* monster, int rounds, Player** targets, int targets_n); // 40