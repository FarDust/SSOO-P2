#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define ACTIVATE_PROMPT 6
#define GET_ENTITIES 7
#define PLAYER_TURN 8

void client_send_message(int client_socket, int pkg_id, char * message);
int client_receive_id(int client_socket);
char * client_receive_payload(int client_socket);
