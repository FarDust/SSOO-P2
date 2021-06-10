#include <stdlib.h>

typedef struct entity
{
  size_t uuid;
  size_t health;
  unsigned int buff[256];
} Entity;
