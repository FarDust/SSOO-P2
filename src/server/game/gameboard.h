#include "../conection.h"


void next_round(Informacion_juego * informacion_juego);
void set_client_prompt();
void send_targets_info();
Slot get_action_info();
size_t get_target_uuid();
void send_updated_info();
void play_turn(Player* player, Informacion_juego * informacion_juego);
void end_condition(GameStatus *status);