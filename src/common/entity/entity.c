#include "entity.h"
#include <stdlib.h>

Entity *ENTITIES[256];
size_t current_entity = 0;

Entity * spawn_entity(){
  Entity *entity = malloc(sizeof(Entity));
  entity->health = 0;
  entity->max_health = 0;
  entity->uuid = current_entity;
  ENTITIES[current_entity] = entity;
  current_entity += 1;
  return entity;
}