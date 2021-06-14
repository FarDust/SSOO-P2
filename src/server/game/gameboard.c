#include "gameboard.h"

size_t LEADER = 0;

void next_round(Informacion_juego * informacion_juego, Informacion_conectar* sockets){
  Player *current_player;
  printf("asddasads\n");
  current_player = informacion_juego->jugadores[LEADER];
  printf("asddasads1.2\n");
  play_turn(current_player, informacion_juego->status, sockets);
  printf("asddasads2\n");
  for (size_t player = 0; player < PLAYER_NUMBER; player++)
  {
    if (player != LEADER){
      play_turn(current_player, informacion_juego->status, sockets);
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

void set_client_prompt(){
  /* Activa el turno para el cliente */
}

void send_targets_info(){
  /* Enviá el numero de objetivos */
  /* Enviá un mensaje por objetivo disponible (vida > 0) codificación de bytes
  el UUID [4 bytes]
  el tipos [1 byte]
  su clase [4 bytes]
  vida actual [4 bytes]
  buffs [256 bytes]*/

  // Objetivos disponibles PLAYERS + MONSTERS
}

Slot get_action_info(){
  /* Espera a que el cliente responda o se desconecte->(Rendirse) */
  return flee;
}

size_t get_target_uuid(){
  /* Espera a que el cliente responda con un Spell o se desconecte->(Rendirse) */
  return 0;
}

void send_updated_info(){
  /* Le indica al cliente que hay nueva información para una UUID*/
}

void send_player_info(Player* player){
  /* Sends current player info to the client */
}

void play_turn(Player* player, GameStatus * status, Informacion_conectar* sockets){
  if (player->properties->health > 0){
    set_client_prompt(); // Notify client that is his turn

    send_targets_info(); // Send available target 

    send_player_info(player); // Send current player info

    size_t objetive_uuid = get_target_uuid(); // obtain target uuid

    Slot slot = get_action_info();  // Get spell from client
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
