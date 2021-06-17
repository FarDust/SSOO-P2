#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "conection.h"
#include "game/gameboard.h"


int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se crea el servidor y se obtienen los sockets de los clientes.
  Informacion_juego * informacion_juego = prepare_sockets_and_get_clients(IP, PORT);

  while ( (informacion_juego != NULL) & !informacion_juego->ready)
  {
    sleep(1);
  }

  pthread_cancel(informacion_juego->informacion_conexiones->main_connector);

  if (informacion_juego->status->monster == NULL){
    informacion_juego->status->monster = spawn_monster(get_random_monster());
  }
  
  for (int i = 0; i<get_player_count(); i++)
  {
    pthread_cancel(informacion_juego->informacion_conexiones->escuchadores[i]);
  }

  printf("Empezando partida!\n");
  while (!end_condition(informacion_juego->status)){ // Mientras end-condition == false
    next_round(informacion_juego);
    printf("[Server]: Se terminó la ronda %ld\n", informacion_juego->status->round);
  }

  for (size_t i = 0; i < get_player_count(); i++)
  {
    server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], END_CONENCTION, "cerrando connexion al tablero\n");
  }

  reset_entities();
  reset_players();
  reset_monster();
  free(informacion_juego->informacion_conexiones);
  free(informacion_juego->status);
  free(informacion_juego);

  return 0;
}
