#include "../../common/player/player.h"
#include "../../common/monster/monster.h"
#include "../conection.h"

typedef struct gameStatus
{
  size_t round;
  Player **players;
  Monster **monsters;
} GameStatus;

void next_round(GameStatus *status, Informacion_conectar sockets);
void set_client_prompt();
void send_targets_info();
Slot get_action_info();
size_t get_target_uuid();
void send_updated_info();
void play_turn(Player *player, GameStatus *status, Informacion_conectar sockets);
