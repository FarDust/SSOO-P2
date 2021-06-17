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


  Monster* monster =  informacion_juego->status->monster;
  select_monster_spell(monster);
  char * server_msg = (char *)cast_monster_spell(monster, player_list, get_player_count(), informacion_juego->status->round);
  for (size_t i = 0; i < get_player_count(); i++)
  {
    server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], STANDARD_MESSAGE, server_msg);
  }
  free(server_msg);

  for (size_t entity = 0; entity < get_entities_number(); entity++)
  {
    Entity* rentity = get_entity(entity);
    if (rentity->buff[Sangrado] > 0){
      char * result = (char *)sangrado(rentity);
      free(result);
    }
    if (rentity->buff[Desmoralized] > 0){
      rentity->buff[Desmoralized] -= 1;
    }
    if (rentity->buff[JumpBlocked] > 0){
      rentity->buff[JumpBlocked] -= 1;
    }
    if (rentity->buff[Toxic] > 0){
      rentity->buff[Toxic] -= 1;
      char * result = (char *)do_dmg(rentity, 400);
      free(result);
      //send message toxic
    }
    if (rentity->buff[AttackBuff] > 0){
      rentity->buff[AttackBuff] -= 1;
    }
  }

  char message[64];
  sprintf(message, "Termina ronda %ld\n", informacion_juego->status->round);
  for (size_t i; i<player_count; i++)
  {
    server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], STANDARD_MESSAGE, message);
  }
  informacion_juego->status->round += 1;
  
}

void broadcast_player_turn(Player* player, Informacion_conectar *conexiones){
  // Notify players the current player turn
  for (size_t i = 0; i < get_player_count(); i++)
  {
    char message[64];
    sprintf(message, "Turno del jugador %s\n", player->properties->name);
    server_send_message(conexiones->sockets_clients[i], STANDARD_MESSAGE, message);
    printf("[Server]: anunciando el turno del player %s al socket %d\n", player->properties->name, conexiones->sockets_clients[i]);
  }
}

void set_client_prompt(int socket){
  // Notify client that is his turn
  server_send_message(socket, ACTIVATE_PROMPT, "\n");
  printf("[Server]: activando el turno del socket %d\n", socket);
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
  size_t total_targets = 0;
  for (size_t i = 0; i < get_player_count(); i++){
    Player *player = informacion_juego->status->players[i];
    if ( player->properties->health > 0){
      total_targets += 1;
    }
  }

  if (informacion_juego->status->monster->properties->health > 0){
    total_targets += 1;
  }
  
  unsigned char buffer[4];
  buffer[0] = (total_targets >> 24) & 0xFF;
  buffer[1] = (total_targets >> 16) & 0xFF;
  buffer[2] = (total_targets >> 8) & 0xFF;
  buffer[3] = total_targets & 0xFF;

  server_send_bytes(player_socket, GET_ENTITIES, 4, buffer);
  printf("[Server]: enviada la cantidad de objetivos %ld\n", total_targets);
  for (size_t i = 0; i < get_player_count(); i++)
  {
    size_t package_len = 4 + 1 + 4 + 4 + MAX_BUFFS;
    Player *player = informacion_juego->status->players[i];
    Entity* entity = informacion_juego->status->players[i]->properties;
    if (player->properties->health > 0){

      char * p_name = player->properties->name;
      unsigned char entity_buffer[package_len + strlen(p_name)];

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
        entity_buffer[buff] = entity->buff[buff-13] & 0xFF;
      }

      entity_buffer[13 + MAX_BUFFS] = strlen(p_name) & 0xFF;

      memcpy(&entity_buffer[13 + MAX_BUFFS + 1], p_name, strlen(p_name));

      server_send_bytes(player_socket, AVAILABLE_TARGET, package_len + strlen(p_name), entity_buffer);
      printf("[Server]: enviado objetivo %ld\n", entity->uuid);
    }
  }

  size_t package_len = 4 + 1 + 4 + 4 + MAX_BUFFS;
  Monster *monster = informacion_juego->status->monster;
  Entity* entity = informacion_juego->status->monster->properties;
  if (monster->properties->health > 0){

    char * p_name = monster->properties->name;
    unsigned char entity_buffer[package_len + strlen(p_name)];

    entity_buffer[0] = (entity->uuid >> 24) & 0xFF;
    entity_buffer[1] = (entity->uuid >> 16) & 0xFF;
    entity_buffer[2] = (entity->uuid >> 8) & 0xFF;
    entity_buffer[3] = entity->uuid & 0xFF;

    entity_buffer[4] = 0x02; // Monster

    entity_buffer[5] = (monster->name >> 24) & 0xFF;
    entity_buffer[6] = (monster->name >> 16) & 0xFF;
    entity_buffer[7] = (monster->name >> 8) & 0xFF;
    entity_buffer[8] = monster->name & 0xFF;

    entity_buffer[9] = (entity->health >> 24) & 0xFF;
    entity_buffer[10] = (entity->health >> 16) & 0xFF;
    entity_buffer[11] = (entity->health >> 8) & 0xFF;
    entity_buffer[12] = entity->health & 0xFF;

    for (size_t buff = 13; buff < package_len; buff++)
    {
      entity_buffer[buff] = entity->buff[buff-13] & 0xFF;
    }

    entity_buffer[13 + MAX_BUFFS] = strlen(p_name) & 0xFF;

    memcpy(&entity_buffer[13 + MAX_BUFFS + 1], p_name, strlen(p_name));

    server_send_bytes(player_socket, AVAILABLE_TARGET, package_len + strlen(p_name), entity_buffer);
    printf("[Server]: enviado objetivo %ld\n", entity->uuid);
  }
}

Slot get_action_info(int socket){
  /* Espera a que el cliente responda o se desconecte->(Rendirse) */
  bool not_listo = true;
  int slot;
  while(not_listo){
    int msg_code = server_receive_id(socket);
    
    if (msg_code == RECEIVE_SPELL) 
    {
      unsigned char * message = (unsigned char *)server_receive_payload(socket);
      slot = (message[0] << 24) | (message[1] << 16) | (message[2] << 8) | (message[3]);
      not_listo = false;
      free(message);
    } else if (msg_code != 0) {
      char * message = server_receive_payload(socket);
      printf("-> Paquete sin sentido recepcionado con \nmsg_code: %d\n message: %s\n",msg_code, message);
      free(message);
    }
  }
  printf("[Server]: spell slot selected: %d\n", slot);

  return slot;
}

size_t get_target_uuid(int socket){
  bool not_listo = true;
  size_t ret_uuid;
  while(not_listo){
   int msg_code = server_receive_id(socket);
    if (msg_code == RECEIVE_UUID) //Recepción del nombre, son todos validos
    {
      
      unsigned char * uuid = (unsigned char *)server_receive_payload(socket);
      ret_uuid = (uuid[0] << 24) | (uuid[1] << 16) | (uuid[2] << 8) | (uuid[3]);
      not_listo = false;
      free(uuid);
    } else if (msg_code != 0) {
      char * message = server_receive_payload(socket);
      printf("-> Paquete sin sentido recepcionado con \nmsg_code: %d\n message: %s\n",msg_code, message);
      free(message);
    }
  }
  /* Espera a que el cliente responda con un Spell o se desconecte->(Rendirse) */
  printf("[Server]: target uuid: %ld\n", ret_uuid);
  
  return ret_uuid;
}

void send_updated_info(){
  /* Le indica al cliente que hay nueva información para una UUID*/
}

void send_player_info(Player* player, int socket){
  /* Sends current player info to the client */
  //uuid vida, nombre spec.
  char * p_name = player->properties->name;
  size_t package_len = 4 + 1 + 4 + 4 + MAX_BUFFS + 1 + strlen(p_name);
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
    entity_buffer[buff] = entity->buff[buff-13] & 0xFF;
  }

  entity_buffer[13 + MAX_BUFFS] = strlen(p_name) & 0xFF;

  memcpy(&entity_buffer[13 + MAX_BUFFS + 1], p_name, strlen(p_name));
  
  server_send_bytes(socket, GET_INFO, package_len, entity_buffer);
  printf("[Server]: enviado datos del player %s\n", entity->name);
}

void play_turn(Player* player, size_t player_index, Informacion_juego * informacion_juego){
  if (player->properties->health > 0){
    int player_socket = informacion_juego->informacion_conexiones->sockets_clients[player_index];

    broadcast_player_turn(player, informacion_juego->informacion_conexiones);

    set_client_prompt(player_socket);

    send_targets_info(player, player_index, informacion_juego); // Send available target 

    char message[] = "[Server]: Selecciona un objetivo!\n";
    server_send_message(player_socket, PLAYER_TURN, message);
    printf("%s", message);

    send_player_info(player, player_socket); // Send current player info

    size_t objetive_uuid = get_target_uuid(player_socket); // obtain target uuid

    Slot slot = get_action_info(player_socket);  // Get spell from client
    if (slot == flee){
      goto finish_turn;
    }

    Entity *target = get_entity_by(objetive_uuid);
    
    select_spell(player, slot);
    char * result = cast_spell(player->properties, target, player->current_spell);
    // TODO: send feedback to all players of actions
    free(result);
    finish_turn:;
    char end_message[64];
    sprintf(end_message, "[Server]: Termino el turno del jugador %s\n", player->properties->name);
    for (size_t i; i < get_player_count(); i++)
    {
      server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], END_TURN, message);
    }
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
  Monster *monster = status->monster;
  if (monster->properties->health == 0){
    end_monsters = true;
  }
  return end_monsters | end_players;
}