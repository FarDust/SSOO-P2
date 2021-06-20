#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "conection.h"
#include "game/gameboard.h"


int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = argv[2]; // 0.0.0.0
  int PORT = atoi(argv[4]); // 8080

  bool continue_playing[5];
  bool new_round = true;

  // Se crea el servidor y se obtienen los sockets de los clientes.
  Informacion_juego * informacion_juego = prepare_sockets_and_get_clients(IP, PORT);

  while(new_round){


    while ( (informacion_juego != NULL) & !informacion_juego->ready)
    {
      sleep(1);
    }
  
    if (informacion_juego->status->monster == NULL){
      informacion_juego->status->monster = spawn_monster(get_random_monster());
    }

    printf("Empezando partida!\n");
    while (!end_condition(informacion_juego->status)){ // Mientras end-condition == false
      next_round(informacion_juego);
      printf("[Server]: Se terminó la ronda %ld\n", informacion_juego->status->round);
    }



    // Ver ganador
    bool vivo_player = false;
    bool vivo_monstruo = false;
    for (size_t i = 0; i < get_player_count(); i++)
    {
      if (informacion_juego->status->players[i]->properties->health > 0){
        vivo_player = true;
        break;
      }
    }
    if (informacion_juego->status->monster->properties->health > 0){
      vivo_monstruo = true;
    }

    if (vivo_monstruo)
    {
      char * message = (char*)malloc(64 * sizeof(char));

      sprintf(message, "El monstruo %s gana la partida\n", informacion_juego->status->monster->properties->name);
      for (size_t i = 0; i < get_player_count(); i++)
      {
        server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], END_CONENCTION, message);
      }
      //free(message);
    } else {

      for (size_t i = 0; i < get_player_count(); i++)
      {
        server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], END_CONENCTION, "Los jugadores han ganado la partida.");
      }
    }

    //Enviar cierre juego
    for (size_t i = 0; i < get_player_count(); i++)
    {
      server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], CONTINUE_PLAYING, "Cerrando connexion al tablero\n");
      new_round = false;
    }

  }

  pthread_cancel(informacion_juego->informacion_conexiones->main_connector);

  for (int i = 0; i<get_player_count(); i++)
  {
    pthread_cancel(informacion_juego->informacion_conexiones->escuchadores[i]);
  }

  //Enviar cierre juego
  for (size_t i = 0; i < get_player_count(); i++)
  {
    server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], END_CONENCTION, "Cerrando connexion al tablero\n");
  }

  
  reset_entities();
  reset_players();
  reset_monster();
  free(informacion_juego->informacion_conexiones);
  free(informacion_juego->status);
  free(informacion_juego);

  return 0;
}
