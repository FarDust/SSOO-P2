#include "../conection.h"


void next_round(Informacion_juego * informacion_juego);
void set_client_prompt(int socket);
void send_targets_info();
Slot get_action_info();
size_t get_target_uuid();
void send_updated_info();
void play_turn(Player *player, size_t player_index, Informacion_juego *informacion_juego);
bool end_condition(GameStatus *status);
