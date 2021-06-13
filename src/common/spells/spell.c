#include "spell.h"

int do_dmg(Entity* target, int dmg)
{
    int dmg_done;
	if (target->health < dmg)
	{
        dmg_done = target->health;
		target->health = 0;
        return dmg_done;
	} else {
		target->health -= dmg;
        return dmg;
	}
}