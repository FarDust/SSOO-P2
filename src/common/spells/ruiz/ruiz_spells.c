#include <math.h>
#include "ruiz_spells.h"
#include "../spell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char* caso_de_copia(Entity* monster, int class_type, Player* to_copy, Entity* target)
{
  time_t t;
  srand((unsigned) time(&t));
  monster->buff[CasoDeCopiaStatus] = 1;
  Slot slot = rand() % 3; // el slot del spell a copiar
  Spell spell = get_spell_slot(to_copy->spec, slot);
  char msg[200];
  printf("Ha copiado la habilidad: %s\n", get_spell_name(spell));
  sprintf(msg, "Ruiz ha copiado la habilidad: %s\n%s",get_spell_name(spell), cast_spell(monster, target, spell));
  monster->buff[CasoDeCopiaStatus] = 0;
  return msg;
}

char* reprobatron(Entity* monster, Entity* target)
{
	target->buff[Desmoralized] = 2;
  char msg[100];
  sprintf(msg, "Ruiz ha desmoralizado a %s\n", target->name);
  return msg;
}

char* sudoRmRf(Entity* monster, int rounds, Entity** targets, int targets_n)
{
  char msg[200];
  sprintf(msg,"Ruiz usó sudo rm-rf\n");
	for (int i = 0; i < targets_n; ++i)
	{
		sprintf(msg, "%s%s", msg, do_dmg(targets[i], apply_buffs(monster, 100*rounds));
	}
  return msg;
}

