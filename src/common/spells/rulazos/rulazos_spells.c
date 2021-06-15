#include <math.h>
#include "rulazos_spells.h"
#include "../spell.h"

void salto(Entity* monster, Entity* target)
{
	do_dmg(target, 1500);
	monster->buff[JumpBlocked] = 1;
};
void espina_venenosa(Entity* target)
{
	if (target->buff[Toxic] < 0){
		do_dmg(target, 500);
	} else {
		target->buff[Toxic] = 3;
	}
};