#include "gameboard.h"
#include "../conection.h"
#include "../../common/entity/entity.h"
#include "../comunication.h"
#include "../utility/utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

size_t get_amount_of_objetives(int server_socket){
  size_t amount_of_objetives;
  unsigned char *message = client_receive_payload(server_socket);
  amount_of_objetives = (message[0] << 24) | (message[1] << 16) | (message[2] << 8) | (message[3]);
  free(message);
  return amount_of_objetives;
  }

Entity * get_target_info(int server_socket){
  /* TODO: Obtains the info of a target a creates an entity */
  bool server_response = false;
  Entity *entity;
  while (!server_response){

    int msg_code = client_receive_id(server_socket);
    if (msg_code == AVAILABLE_TARGET){
      /*
      el UUID [4 bytes] 0 1 2 3
      el tipos [1 byte] 4
      su clase [4 bytes] 5 6 7 8
      vida actual [4 bytes] 9 10 11 12
      buffs [MAX_BUFFS bytes] 13>
      */

      server_response = true;
      
      unsigned char *message = client_receive_payload(server_socket);
      size_t package_len = 4 + 1 + 4 + 4 + MAX_BUFFS + 1; //Nota: tamaño maximo nombre = 64
      int name_len = message[13 + MAX_BUFFS];
      package_len += name_len;
      unsigned char entity_buffer[package_len];
      memcpy(entity_buffer, message, package_len);
      free(message);

      

      if (entity_buffer[4] == 0x01){
        Player *player = spawn_player();
        size_t spec = (entity_buffer[5] << 24) | (entity_buffer[6] << 16) | (entity_buffer[7] << 8) | (entity_buffer[8]);
        set_player_class(player, spec);
        entity = player->properties;
      }
      else if (entity_buffer[4] == 0x02)
      {
        size_t spec = (entity_buffer[5] << 24) | (entity_buffer[6] << 16) | (entity_buffer[7] << 8) | (entity_buffer[8]);
        Monster *monster = spawn_monster(spec);
        entity = monster->properties;
        
      }

      entity->uuid = (entity_buffer[0] << 24) | (entity_buffer[1] << 16) | (entity_buffer[2] << 8) | (entity_buffer[3]);
      entity->health = (entity_buffer[9] << 24) | (entity_buffer[10] << 16) | (entity_buffer[11] << 8) | (entity_buffer[12]);
      for (size_t buff = 13; buff < 13 + MAX_BUFFS; buff++)
      {
        entity->buff[buff-13] = entity_buffer[buff];
      }

      
      
      char * name = calloc(name_len + 1, sizeof(char));
      memcpy(name, &entity_buffer[13 + MAX_BUFFS + 1], name_len + 1);
      name[name_len] = 0x00;
      entity->name = name;

    } else {
      unsigned char *message = client_receive_payload(server_socket);
      free(message);
    }
  }
  return entity;
}

void get_targets_info(int server_socket){
  size_t objetives = get_amount_of_objetives(server_socket);
  for (size_t target = 0; target < objetives; target++)
  {
    Entity *current_entity = get_target_info(server_socket);
    printf("Opción - %ld \n", current_entity->uuid);
    show_status(current_entity, false);
  }
}

size_t get_player_option(){
  printf("Selecciona un objetivo de la lista anterior:");
  char *input = get_input();
  size_t result = atoi(input);
  free(input);
  return result;
}

void send_target(int server_socket, size_t uuid){

  /* TODO: sends uuid to the server */
  //transformar size_t en char*

  unsigned char buffer[4];
  buffer[0] = (uuid >> 24) & 0xFF;
  buffer[1] = (uuid >> 16) & 0xFF;
  buffer[2] = (uuid >> 8) & 0xFF;
  buffer[3] = uuid & 0xFF;

  client_send_bytes(server_socket, RECEIVE_UUID, 4, buffer);
}

Player * get_own_data(int server_socket){
  bool not_listo = true;
  Player *player;
  while (not_listo)
  {
    int msg_code = client_receive_id(server_socket);
    if (msg_code == GET_INFO){
      
      player = spawn_player();
      /* TODO: Obtain player data from server and fill it*/
      /* TODO: use bytes in message to spawn player in client*/
      unsigned char *message = client_receive_payload(server_socket);
      player->properties->uuid = (message[0] << 24) | (message[1] << 16) | (message[2] << 8) | (message[3]);
      player->spec = (message[5] << 24) | (message[6] << 16) | (message[7] << 8) | (message[8]);
      player->properties->health = (message[9] << 24) | (message[10] << 16) | (message[11] << 8) | (message[12]);
      
      for (size_t buff = 13; buff < 13 + MAX_BUFFS; buff++)
      {
        player->properties->buff[buff-13] = message[buff];
      }
      int name_len = message[13 + MAX_BUFFS];
      char * name = calloc(name_len + 1, sizeof(char));
      memcpy(name, &message[13 + MAX_BUFFS + 1], name_len + 1);
      player->properties->name = name;
      
      not_listo = false;
      set_player_class(player, player->spec);
      player->properties->health = (message[9] << 24) | (message[10] << 16) | (message[11] << 8) | (message[12]);
      free(message);
      show_status(player->properties, true);
    }
  
  
  }
  return player;
}

void send_spell(int server_socket, Slot slot){
  /* Sends Slot number to the server */
  
  unsigned char buffer[4];
  buffer[0] = (slot >> 24) & 0xFF;
  buffer[1] = (slot >> 16) & 0xFF;
  buffer[2] = (slot >> 8) & 0xFF;
  buffer[3] = slot & 0xFF;

  int option = RECEIVE_SPELL;
  client_send_bytes(server_socket,option,4,buffer);
  printf("[Client] Enviando hechizo %d al servidor\n", slot);
}

void select_targets(int server_socket){
  /* TODO: select a target from list*/
  Player *player = get_own_data(server_socket);
  size_t uuid = get_player_option();
  printf("\n");
  Entity *entity = get_entity_by(uuid);
  send_target(server_socket, uuid);

  printf("Seleccionado como objetivo con uuid %ld\n", entity->uuid);

  bool waiting_taunt_request = true;
  while (waiting_taunt_request){
    int msg_code = client_receive_id(server_socket);
    if ( msg_code == EVENT){
      char *message = (char *)client_receive_payload(server_socket);
      printf("[Server]: %s\n", message);
      free(message);
      waiting_taunt_request = false;
    } else  if (msg_code != 0){
      char *message = (char *)client_receive_payload(server_socket);
      printf("[Client]: Mensaje sin sentido recibido\nmsg_code: %d\nmessage: %s\n", msg_code, message);
      free(message);
      goto free_temp_game;
    } else {
      printf("[Client]: Servidor desconectado\n");
      goto free_temp_game;
    }
  }
  show_spells(player);
  printf("Otro - Abandonar la partida\n");

  printf("Selecciona un hechizo: ");
  char *input = get_input();

  Slot slot = atoi(input);

  if ( (strcmp(input, "0") != 0) & (slot == 0) ){
    free(input);
    printf("Haz abandonado la partida, seras un espectador!\n");
    send_spell(server_socket, flee);
    goto free_temp_game;
  }
  free(input);
  send_spell(server_socket, slot);

  // Cast fake spell in client side to prompt results
  char * result = cast_spell(player->properties, entity, get_spell_slot(player->spec, slot));
  free(result);
  free_temp_game:;
  reset_entities();
  reset_players();
  reset_monster();
}

void player_turn_watcher(int server_socket){
  while (true)
  {
    int msg_code = client_receive_id(server_socket);
    if (msg_code == GET_ENTITIES){
      get_targets_info(server_socket);
    } else if (msg_code == PLAYER_TURN) {
      char * message = (char *)client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);
      select_targets(server_socket);
      break;
    } else if (msg_code == SKIP_FASE){
      break;
    } else if (msg_code == STANDARD_MESSAGE) {
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> El servidor anuncia: %s\n", message);
      free(message);
    } else if (msg_code == GAME_MESSAGE) {
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> El servidor anuncia: %s\n", message);
      free(message);
    } else if (msg_code == EVENT) {
      char * message = (char *)client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);
    } else if (msg_code != 0){
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> Paquete sin sentido recepcionado con \nmsg_code: %d\n message: %s\n",msg_code, message);
      free(message);
    }
  }
}