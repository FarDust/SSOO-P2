#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "conection.h"


int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se crea el servidor y se obtienen los sockets de los clientes.
  Informacion_conectar * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // Se crean los 5 threads con la informacion necesaria para cada uno y su socket a escuchar.
  bool no_creados = true;
  Informacion_juego* informacion_thread = malloc(sizeof(Informacion_juego));
  informacion_thread->informacion_conexiones = players_info;
  informacion_thread->jugadores = get_player_list();
  informacion_thread->status = malloc(sizeof(GameStatus));
  

  for(int i=0; i<PLAYER_NUMBER ;i++)
  {
    informacion_thread->attention = i;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, Conexion, informacion_thread);
    players_info->escuchadores[i] = thread_id;
  }


  while (1)
  {
    
  }

  return 0;
}
