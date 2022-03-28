#include <math.h>
#include <stdio.h>
#include "great_jagruz_spells.h"
#include "../spell.h"

const char* ruzgar(Entity* monster, Entity* target)
{	
  char* str;
  str = (char *)calloc(200, 1);
	char msg[200];
  char* response = do_dmg(target, apply_buffs(monster, 1000));
	sprintf(msg, "Great Jagruz ha usado ruzgar\n%s", response);
  write_message(str, msg);
  free(response);
  return str;
}

const char* coletazo(Entity* monster, Entity** targets, int targets_n)
{
  char* str;
  str = (char *)calloc(200, 1);
	char msg[200];
  char* response;
  sprintf(msg, "Great Jagruz ha usado coletazo\n");
	for (int i = 0; i < targets_n; ++i)
	{
    response = do_dmg(targets[i], apply_buffs(monster, 500));
		sprintf(msg,"%s%s", msg, response);
    free(response);
	}
  write_message(str, msg);
  return str;
}