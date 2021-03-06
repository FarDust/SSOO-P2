#pragma once


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
#include "game/structs_gameboard.h"

//http://manpages.ubuntu.com/manpages/trusty/man7/sys_socket.h.7posix.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html

typedef struct informacion_conectar{
  int sockets_clients[PLAYER_NUMBER];
  int server_socket;
  bool conexiones[PLAYER_NUMBER];
  pthread_t escuchadores[PLAYER_NUMBER];
  pthread_t main_connector;
} Informacion_conectar;

typedef struct informacion_juego{
  Informacion_conectar* informacion_conexiones;
  Player** jugadores;
  GameStatus* status;
  int attention;
  bool continue_playing[PLAYER_NUMBER];
  bool ready;
  bool respondieron[PLAYER_NUMBER];
} Informacion_juego;



Informacion_juego * prepare_sockets_and_get_clients(char * IP, int port);
void *Th_conectador(Informacion_juego * info_juego);
void *Conexion(Informacion_juego * informacion_thread);
char * revert(char * message);