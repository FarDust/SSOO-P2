#include <math.h>
#include "rulazos_spells.h"
#include "../spell.h"

char* salto(Entity* monster, Entity* target)
{
	char msg[100];
	sprintf(msg,"Rulazos usó salto\n%s", do_dmg(target, apply_buffs(monster, 1500)));
	monster->buff[JumpBlocked] = 2;
};
char* espina_venenosa(Entity* monster, Entity* target)
{
	char msg[100];
	sprintf(msg, "Rulazos usó espina venenosa\n",)
	if (target->buff[Toxic] > 0){
		sprintf(msg, "%s%s ya estaba envenedado\n%s", msg, target->name,do_dmg(target,  apply_buffs(monster, 500)));
	} else {
		target->buff[Toxic] = 3;
		sprintf(msg, "%s%s ha sido envenedado\n", msg, target->name);
	}
	return msg;
};