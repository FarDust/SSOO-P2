#include "hunter_spells.h"
#include "../spell.h"

void estocada(Entity *target){
  target->health -= 1000;
  target->buff[Sangrado] += 1;
};