#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "conection.h"
#include "game/gameboard.h"


int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = argv[2]; // 0.0.0.0
  int PORT = atoi(argv[4]); // 8080

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

    informacion_juego->ready = false;

    // Ver ganador
    bool vivo_monstruo = false;
    if (informacion_juego->status->monster->properties->health > 0){
      vivo_monstruo = true;
    }
    
    for (size_t i = 0; i < MAX_BUFFS; i++)
    {
      informacion_juego->status->monster->properties->buff[i] = 0;
    }
    

    if (vivo_monstruo)
    {
      char * message = (char*)malloc(64 * sizeof(char));

      sprintf(message, "El monstruo %s gana la partida\n", informacion_juego->status->monster->properties->name);
      for (size_t i = 0; i < get_player_count(); i++)
      {
        if(informacion_juego->informacion_conexiones->conexiones[i]){
        server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], CONTINUE_PLAYING, message);
        }
      }
      free(message);
    } else {
      for (size_t i = 0; i < get_player_count(); i++)
      {
        if(informacion_juego->informacion_conexiones->conexiones[i]){
        server_send_message(informacion_juego->informacion_conexiones->sockets_clients[i], CONTINUE_PLAYING, "Los jugadores han ganado la partida.\n");
        }
      }
    }

    //poner while true hasta que todos contesten

    new_round = false;
    bool no_respondieron = true;
    while (no_respondieron)
    {
      no_listo:;
      no_respondieron = false;
      for (size_t i = 0; i < PLAYER_NUMBER; i++)
      {
        if(informacion_juego->informacion_conexiones->conexiones[i]){
          if (informacion_juego->respondieron[i] == false)
          {
            goto no_listo;
          }
          
        }

      }
    }

    for (size_t i = 0; i < PLAYER_NUMBER; i++)
      {
        if(informacion_juego->informacion_conexiones->conexiones[i]){
          if (informacion_juego->continue_playing[i])
          {
            new_round = true;
            break;
          }
          
        }
        
      }
  }

  int result;
  

  pthread_cancel(informacion_juego->informacion_conexiones->main_connector);
  
  for (int i = 0; i<get_player_count(); i++)
  {
    pthread_cancel(informacion_juego->informacion_conexiones->escuchadores[i]);
    pthread_join(informacion_juego->informacion_conexiones->escuchadores[i], &result);
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
