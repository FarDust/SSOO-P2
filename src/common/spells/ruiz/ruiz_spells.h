#pragma once
#include "../../entity/entity.h"
#include "../../player/player.h"

void caso_de_copia(Entity* monster, int class_type, Player* to_copy, Entity* target);
void reprobatron(Entity* target);
void sudoRmRf(int rounds, Entity** targets, int targets_n);
