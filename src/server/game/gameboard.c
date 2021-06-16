#include "gameboard.h"
#include <stdbool.h>
#include <string.h>

size_t LEADER = 0;

void next_round(Informacion_juego * informacion_juego){
  Player *current_player;
  Player **player_list = informacion_juego->jugadores;
  current_player = player_list[LEADER];
  play_turn(current_player, LEADER, informacion_juego);
  size_t player_count = get_player_count();
  for (size_t player = 0; player < player_count; player++)
  {
    if (player != LEADER){
      current_player = player_list[player];
      play_turn(current_player, player , informacion_juego);
    }
  }

  /* MONSTER CODE
   CODE
  */
  

  for (size_t entity = 0; entity < get_entities_number(); entity++)
  {
    if (get_entity(entity)->buff[Sangrado] > 0){
      sangrado(get_entity(entity));
    }
  }
  
  informacion_juego->status->round += 1;
}

void broadcast_player_turn(Player* player, Informacion_conectar *conexiones){
  // Notify players the current player turn
  for (size_t i = 0; i < get_player_count(); i++)
  {
    char message[64];
    sprintf(message, "Turno del jugador %s/n", player->properties->name);
    server_send_message(conexiones->sockets_clients[i], STANDARD_MESSAGE, message);
  }
}

void set_client_prompt(int socket){
  // Notify client that is his turn
  server_send_message(socket, ACTIVATE_PROMPT, "/n");
}

void send_targets_info(Player *player, size_t player_index, Informacion_juego * informacion_juego){
  /* Enviá el numero de objetivos */
  /* Enviá un mensaje por objetivo disponible (vida > 0) codificación de bytes
  el UUID [4 bytes]
  el tipos [1 byte]
  su clase [4 bytes]
  vida actual [4 bytes]
  buffs [MAX_BUFFS bytes]*/

  int player_socket = informacion_juego->informacion_conexiones->sockets_clients[player_index];

  // Objetivos disponibles PLAYERS + MONSTERS
  size_t total_targets = get_player_count(); // + monsters_count()
  unsigned char buffer[4];
  buffer[0] = (total_targets >> 24) & 0xFF;
  buffer[1] = (total_targets >> 16) & 0xFF;
  buffer[2] = (total_targets >> 8) & 0xFF;
  buffer[3] = total_targets & 0xFF;

  server_send_bytes(player_socket, GET_ENTITIES, 4, buffer);
  for (size_t i = 0; i < get_player_count(); i++)
  {
    size_t package_len = 4 + 1 + 4 + 4 + MAX_BUFFS;
    unsigned char entity_buffer[package_len];
    Player *player = informacion_juego->status->players[i];
    Entity* entity = informacion_juego->status->players[i]->properties;
    entity_buffer[0] = (entity->uuid >> 24) & 0xFF;
    entity_buffer[1] = (entity->uuid >> 16) & 0xFF;
    entity_buffer[2] = (entity->uuid >> 8) & 0xFF;
    entity_buffer[3] = entity->uuid & 0xFF;

    entity_buffer[4] = 0x01; // PLAYER

    entity_buffer[5] = (player->spec >> 24) & 0xFF;
    entity_buffer[6] = (player->spec >> 16) & 0xFF;
    entity_buffer[7] = (player->spec >> 8) & 0xFF;
    entity_buffer[8] = player->spec & 0xFF;

    entity_buffer[9] = (entity->health >> 24) & 0xFF;
    entity_buffer[10] = (entity->health >> 16) & 0xFF;
    entity_buffer[11] = (entity->health >> 8) & 0xFF;
    entity_buffer[12] = entity->health & 0xFF;

    for (size_t buff = 13; buff < package_len; buff++)
    {
      entity_buffer[buff] = entity->buff[buff] & 0xFF;
    }
    server_send_bytes(player_socket, AVAILABLE_TARGET, package_len, entity_buffer);
  }
  
}

Slot get_action_info(int socket){
  /* Espera a que el cliente responda o se desconecte->(Rendirse) */
  bool not_listo = true;
  int flee;
  while(not_listo){
   int msg_code = server_receive_id(socket);
    
    if (msg_code == RECEIVE_SPELL) //Recepción del nombre, son todos validos
    {
      char * message = server_receive_payload(socket);
      
      flee = atoi(message);
      not_listo = false;
    }
  }


  return flee;
}

size_t get_target_uuid(int socket){
  bool not_listo = true;
  size_t ret_uuid;
  while(not_listo){
   int msg_code = server_receive_id(socket);
    if (msg_code == RECEIVE_UUID) //Recepción del nombre, son todos validos
    {
      char * uuid = server_receive_payload(socket);
      ret_uuid = atoi(uuid);
      not_listo = false;
      
    }
  }
  /* Espera a que el cliente responda con un Spell o se desconecte->(Rendirse) */
  
  return ret_uuid;
}

void send_updated_info(){
  /* Le indica al cliente que hay nueva información para una UUID*/
}

void send_player_info(Player* player, int socket){
  /* Sends current player info to the client */
  //uuid vida, nombre spec.
  char * p_name = player->properties->name;
  size_t package_len = 4 + 1 + 4 + 4 + MAX_BUFFS + strlen(p_name);
  unsigned char entity_buffer[package_len];
  Entity* entity = player->properties;

  entity_buffer[0] = (entity->uuid >> 24) & 0xFF;
  entity_buffer[1] = (entity->uuid >> 16) & 0xFF;
  entity_buffer[2] = (entity->uuid >> 8) & 0xFF;
  entity_buffer[3] = entity->uuid & 0xFF;

  entity_buffer[4] = 0x01; // PLAYER

  entity_buffer[5] = (player->spec >> 24) & 0xFF;
  entity_buffer[6] = (player->spec >> 16) & 0xFF;
  entity_buffer[7] = (player->spec >> 8) & 0xFF;
  entity_buffer[8] = player->spec & 0xFF;

  entity_buffer[9] = (entity->health >> 24) & 0xFF;
  entity_buffer[10] = (entity->health >> 16) & 0xFF;
  entity_buffer[11] = (entity->health >> 8) & 0xFF;
  entity_buffer[12] = entity->health & 0xFF;

  for (size_t buff = 13; buff < 13 + MAX_BUFFS; buff++)
    {
      entity_buffer[buff] = entity->buff[buff] & 0xFF;
    }

  entity_buffer[13 + MAX_BUFFS];

  memcpy(&entity_buffer[13 + MAX_BUFFS], p_name, strlen(p_name));
  
  server_send_bytes(socket, GET_INFO, package_len, entity_buffer);

}

void play_turn(Player* player, size_t player_index, Informacion_juego * informacion_juego){
  GameStatus *status = informacion_juego->status;
  if (player->properties->health > 0){
    int player_socket = informacion_juego->informacion_conexiones->sockets_clients[player_index];

    broadcast_player_turn(player, informacion_juego->informacion_conexiones);

    set_client_prompt(player_socket);

    send_targets_info(player, player_index, informacion_juego); // Send available target 

    send_player_info(player, player_socket); // Send current player info

    size_t objetive_uuid = get_target_uuid(player_socket); // obtain target uuid

    Slot slot = get_action_info(player_socket);  // Get spell from client
    if (slot == flee){
      goto finish_turn;
    }

    Entity *target = get_entity_by(objetive_uuid);
    
    Spell spell = get_spell_slot(player->spec, slot);
    select_spell(player, spell);
    cast_spell(player->properties, target, player->current_spell);
    finish_turn:;
  }
}

bool end_condition(GameStatus *status){
  bool end_players = true;
  Player **players = get_player_list();
  for (size_t player = 0; player < get_player_count(); player++)
  {
    if (players[player]->properties->health > 0){
      end_players = false;
    }
  }
  bool end_monsters = false;
  Monster **monsters = status->monsters;
  /* TODO: Ask end condition for monsters
  for (size_t monster = 0; monster < get_player_count(); monster++)
  {
    if (players[player]->properties->health > 0){
      end_players = false;
    }
  }
  */
  return end_monsters | end_players;
}