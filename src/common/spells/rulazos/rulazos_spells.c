#include <math.h>
#include "rulazos_spells.h"
#include "../spell.h"

void salto(Entity* monster, Entity* target)
{
	do_dmg(target, 1500);
	monster->buff[CanJump] = 0;
};
void espina_venenosa(Monster* monster, Entity* target)
{
	if (target->buff[Toxic] == 1){
		do_dmg(target, 500);
	} else {
		target->buff[Toxic] = 1;
	}
};