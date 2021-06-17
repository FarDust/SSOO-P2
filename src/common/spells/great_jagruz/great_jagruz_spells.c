#include <math.h>
#include "great_jagruz_spells.h"
#include "../spell.h"

char* ruzgar(Entity* monster, Entity* target)
{	
	char msg[100];
	sprintf(msg, "Great Jagruz ha usado ruzgar\n%s", do_dmg(target, apply_buffs(monster, 1000)));
  return msg;
}

char* coletazo(Entity* monster, Entity** targets, int targets_n)
{
  char msg[200];
  sprintf(msg, "Great Jagruz ha usado ruzgar\n");
	for (int i = 0; i < targets_n; ++i)
	{
		sprintf(msg,"%s%s", msg, do_dmg(targets[i], apply_buffs(monster, 500)));
	}
  return msg;
}