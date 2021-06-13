#pragma once

#include <stdlib.h>

typedef struct entity
{
  size_t uuid;
  size_t health;
  size_t max_health;
  unsigned int buff[256];
} Entity;

Entity *spawn_entity();