#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include "comunication.h"


//http://manpages.ubuntu.com/manpages/trusty/man7/sys_socket.h.7posix.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html

typedef struct informacion_conectar{
  int sockets_clients[5];
  int server_socket;
  bool conexiones[5];
  pthread_t escuchadores[5];

} Informacion_conectar;

typedef struct jugador{
  char* nombre;
  char* clase;
} Jugador;

typedef struct informacion_juego{
  Informacion_conectar* informacion_conexiones;
  Jugador* jugador[5];
  int attention;

} Informacion_juego;



Informacion_conectar * prepare_sockets_and_get_clients(char * IP, int port);
void *Th_conectador(Informacion_conectar * info_conectar);
void *Conexion(Informacion_juego * informacion_thread);
char * revert(char * message);