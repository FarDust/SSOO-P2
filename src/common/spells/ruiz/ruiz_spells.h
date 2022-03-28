#pragma once
#include "../../entity/entity.h"
#include "../../player/player.h"

const char* caso_de_copia(Entity* monster, int class_type, Player* to_copy, Entity* target);
const char* reprobatron(Entity* monster, Entity* target);
const char* sudoRmRf(Entity* monster, int rounds, Entity** targets, int targets_n);
