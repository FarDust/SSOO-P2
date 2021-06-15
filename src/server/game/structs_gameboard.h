#include "../../common/monster/monster.h"
#include "../../common/player/player.h"

typedef struct gameStatus
{
  size_t round;
  Player **players;
  Monster **monsters;
} GameStatus;
