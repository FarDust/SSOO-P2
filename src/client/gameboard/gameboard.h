#include "../../common/player/player.h"
#include "../../common/monster/monster.h"

void player_turn_watcher(int server_socket);
void get_targets_info(int server_socket);
void select_targets(int server_socket);
void send_target(int server_socket, char* uuid);