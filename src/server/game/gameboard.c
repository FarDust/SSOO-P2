#include "gameboard.h"

size_t LEADER = 0;

void next_round(Informacion_juego * informacion_juego){
  Player *current_player;
  Player **player_list = informacion_juego->jugadores;
  current_player = player_list[LEADER];
  play_turn(current_player, informacion_juego->status);
  size_t player_count = get_player_count();
  for (size_t player = 0; player < player_count; player++)
  {
    if (player != LEADER){
      current_player = player_list[player];
      play_turn(current_player, informacion_juego->status);
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

void play_turn(Player* player, Informacion_juego * informacion_juego){
  GameStatus *status = informacion_juego->status;
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

void end_condition(GameStatus *status){
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