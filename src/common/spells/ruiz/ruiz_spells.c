#include <math.h>
#include "ruiz_spells.h"
#include "../spell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void caso_de_copia(Entity* monster, int class_type, Player* to_copy, Entity* target)
{
  time_t t;
  srand((unsigned) time(&t));
  monster->buff[CasoDeCopiaStatus] = 1;
  Slot slot = rand() % 3; // el slot del spell a copiar
  Spell spell = get_spell_slot(to_copy->spec, slot);
  printf("Ha copiado la habilidad: %s", get_spell_name(spell));
  cast_spell(monster, target, spell);
  monster->buff[CasoDeCopiaStatus] = 0;
}

void reprobatron(Entity* target)
{
	target->buff[Desmoralized] = 1;
}

void sudoRmRf(int rounds, Entity** targets, int targets_n)
{
	for (int i = 0; i < targets_n; ++i)
	{
		do_dmg(targets[i], 100*rounds);
	}
}

