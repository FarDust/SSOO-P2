#include <math.h>
#include "hunter_spells.h"

void estocada(Entity *target){
  target->health =- (size_t)fmin(target->health, 1000);
  target->buff[Sangrado] += 1;
  target->buff[Sangrado] = (unsigned int)fmin(target->buff[Sangrado], 3);
};