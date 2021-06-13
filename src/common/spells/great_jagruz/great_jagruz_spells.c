#include <math.h>
#include "great_jagruz_spells.h"
#include "../spell.h"

void ruzgar(Entity* target)
{
	do_dmg(target, 1000);
}

void coletazo(Entity** targets, int targets_n)
{
	for (int i = 0; i < targets_n; ++i)
	{
		do_dmg(targets[i], 500);
	}
}