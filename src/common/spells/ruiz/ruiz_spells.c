#include <math.h>
#include "ruiz_spells.h"
#include "../spell.h"

void caso_de_copia(Entity* monster, int class_type, Entity* target)
{
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

