#include "gameboard.h"
#include "../conection.h"
#include "../../common/entity/entity.h"
#include "../comunication.h"
#include "../utility/utility.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

size_t get_amount_of_objetives(int server_socket){
  /* TODO: get amount of valid objetives from server*/
  return 5;
}

Entity * get_target_info(int server_socket){
  /* TODO: Obtains the info of a target a creates an entity */
  Entity *entity = spawn_entity();
  /* Rellenar entity*/
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

void send_target(size_t uuid){
  /* TODO: sends uuid to the server */
}

Player * get_own_data(int server_socket){
  Player *player = spawn_player();
  /* TODO: Obtain player data from server and fill it*/
  unsigned char * message = client_receive_payload(server_socket); // Data of player in bytes
  printf("El servidor envió: %s\n", message);
  /* TODO: use bytes in message to spawn player in client*/
  free(message);
  return player;
}

void send_spell(Slot slot){
  /* TODO: sends Slot number to the server */
}

void select_targets(int server_socket){
  /* TODO: select a target from list*/
  Player *player = get_own_data(server_socket);
  size_t uuid = get_player_option();
  printf("\n");
  Entity *entity = get_entity_by(uuid);
  send_target(uuid);
  show_spells(player); // TODO: show flee slot and manage exceptions
  Slot slot = atoi(get_input()); /* TODO: Obtain slot from player */
  send_spell(slot);

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
    }
  }
}