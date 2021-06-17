#include <math.h>
#include <stdio.h>
#include "rulazos_spells.h"
#include "../spell.h"

const char* salto(Entity* monster, Entity* target)
{
  char* str;
  str = (char *)calloc(200, 1);
	char msg[200];
  char* response = do_dmg(target, apply_buffs(monster, 1500));
	sprintf(msg,"Rulazos usó salto\n%s", response);
	monster->buff[JumpBlocked] = 2;
	write_message(str, msg);
  free(response);
  return str;
};
const char* espina_venenosa(Entity* monster, Entity* target)
{
	char* str;
  str = (char *)calloc(200, 1);
  char msg[200];
  char* response;
	sprintf(msg, "Rulazos usó espina venenosa\n");
	if (target->buff[Toxic] > 0){
    response = do_dmg(target,  apply_buffs(monster, 500));
		sprintf(msg, "%s%s ya estaba envenedado\n%s", msg, target->name, response);
    free(response);
	} else {
		target->buff[Toxic] = 3;
		sprintf(msg, "%s%s ha sido envenedado\n", msg, target->name);
	}
	write_message(str, msg);
  return str;
};