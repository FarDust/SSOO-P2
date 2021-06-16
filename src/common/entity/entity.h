#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define MAX_ENTITIES 256

typedef struct entity
{
  size_t uuid;
  size_t health;
  size_t max_health;
  unsigned int buff[16];
} Entity;

Entity *spawn_entity();
void kill_entity(Entity *entity);
size_t get_entities_number();
void reset_entities();
Entity *get_entity_by(size_t uuid);
Entity *get_entity(size_t entity);
void show_status(Entity * entity, bool with_uuid);