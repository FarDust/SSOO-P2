#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "conection.h"


int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se crea el servidor y se obtienen los sockets de los clientes.
  Informacion_juego * informacion_juego = prepare_sockets_and_get_clients(IP, PORT);

  while (!informacion_juego->ready)
  {
    sleep(1);
  }
  
  for (int i = 0; i<get_player_count(); i++)
  {
    pthread_cancel(informacion_juego->informacion_conexiones->escuchadores[i]);
  }

  printf("Empezando partida!\n");
  int c_rounds = 0;
  while (!end_condition(informacion_juego->status)){ // Mientras end-condition == false
    next_round(informacion_juego);
    printf("[Server]: Se terminó la ronda %d\n", c_rounds);
  }


  return 0;
}
