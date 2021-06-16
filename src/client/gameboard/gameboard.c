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
      size_t package_len = 4 + 1 + 4 + 4 + MAX_BUFFS;
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
      }

      entity->uuid = (entity_buffer[0] << 24) | (entity_buffer[1] << 16) | (entity_buffer[2] << 8) | (entity_buffer[3]);
      entity->health = (entity_buffer[9] << 24) | (entity_buffer[10] << 16) | (entity_buffer[11] << 8) | (entity_buffer[12]);
      for (size_t buff = 13; buff < 13 + MAX_BUFFS; buff++)
      {
        entity->buff[buff] = entity_buffer[buff];
      }
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
  return atoi(get_input());
}

void send_target(int server_socket, char* uuid){

  /* TODO: sends uuid to the server */

  int option = RECEIVE_UUID;
  client_send_message(server_socket,option,uuid);
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
      size_t package_len = strlen(message);
      unsigned char entity_buffer[package_len];
      player->properties->uuid = (message[0] << 24) | (message[1] << 16) | (message[2] << 8) | (message[3]);
      player->spec = (message[5] << 24) | (message[6] << 16) | (message[7] << 8) | (message[8]);
      player->properties->health = (message[9] << 24) | (message[10] << 16) | (message[11] << 8) | (message[12]);
      
      for (size_t buff = 13; buff < 13 + MAX_BUFFS; buff++)
      {
        player->properties->buff[buff] = message[buff];
      }
      char * name = malloc(package_len - 13 + MAX_BUFFS);
      memcpy(name, &message[package_len - 13 + MAX_BUFFS], package_len - 13 + MAX_BUFFS);
      player->properties->name = name;
      
      free(message);
      not_listo = false;
    }
  
  
  }
  return player;
}

void send_spell(int server_socket, Slot slot){
  /* TODO: sends Slot number to the server */
  
  char * message = (char)slot;
  int option = RECEIVE_SPELL;
  client_send_message(server_socket,option,message);
}

void select_targets(int server_socket){
  /* TODO: select a target from list*/
  Player *player = get_own_data(server_socket);
  size_t uuid = get_player_option();
  printf("\n");
  Entity *entity = get_entity_by(uuid);
  send_target(server_socket,uuid);
  show_spells(player); // TODO: show flee slot and manage exceptions
  Slot slot = atoi(get_input()); /* TODO: Obtain slot from player */
  send_spell(server_socket,slot);

  // Cast fake spell in client side to prompt results
  cast_spell(player->properties, entity, get_spell_slot(player->spec, slot));
}

void player_turn_watcher(int server_socket){
  reset_entities();
  while (true)
  {
    int msg_code = client_receive_id(server_socket);
    if (msg_code == GET_ENTITIES){
      get_targets_info(server_socket);
    } else if (msg_code == PLAYER_TURN) {
      select_targets(server_socket);
      break;
    } else if (msg_code == SKIP_FASE){
      break;
    } else if (msg_code == STANDARD_MESSAGE) {
      char * message = client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);
    }
  }
}