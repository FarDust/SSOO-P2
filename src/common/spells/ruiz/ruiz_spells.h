#pragma once
#include "../../entity/entity.h"
#include "../../player/player.h"

char* caso_de_copia(Entity* monster, int class_type, Player* to_copy, Entity* target);
char* reprobatron(Entity* monster, Entity* target);
char* sudoRmRf(Entity* monster, int rounds, Entity** targets, int targets_n);
