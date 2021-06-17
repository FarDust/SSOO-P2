#include <math.h>
#include "ruiz_spells.h"
#include "../spell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const char* caso_de_copia(Entity* monster, int class_type, Player* to_copy, Entity* target)
{
  time_t t;
  srand((unsigned) time(&t));
  monster->buff[CasoDeCopiaStatus] = 1;
  Slot slot = rand() % 3; // el slot del spell a copiar
  Spell spell = get_spell_slot(to_copy->spec, slot);
  char* str;
  str = (char *)calloc(200, 1);
  char msg[200];
  char* response = cast_spell(monster, target, spell);
  sprintf(msg, "Ruiz ha copiado la habilidad: %s\n%s",get_spell_name(spell), response);
  monster->buff[CasoDeCopiaStatus] = 0;
  write_message(str, msg);
  free(response);
  return str;
}

const char* reprobatron(Entity* monster, Entity* target)
{
	target->buff[Desmoralized] = 2;
  char msg[200];
  char* str;
  str = (char *)calloc(200, 1);
  sprintf(msg, "Ruiz ha desmoralizado a %s\n", target->name);
  write_message(str, msg);
  return str;
}

const char* sudoRmRf(Entity* monster, int rounds, Entity** targets, int targets_n)
{
  char msg[200];
  char* str;
  str = (char *)calloc(200, 1);
  char* response;
  sprintf(msg,"Ruiz usó sudo rm-rf\n");
	for (int i = 0; i < targets_n; ++i)
	{
    response = do_dmg(targets[i], apply_buffs(monster, 100*rounds));
		sprintf(msg, "%s%s", msg, response);
    free(response);
	}
  write_message(str, msg);
  return str;
}

