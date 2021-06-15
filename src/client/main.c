#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"
#include "player/prompts.h"
#include "utility/utility.h"
#include "gameboard/gameboard.h"



int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);
  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);

    if (msg_code == STANDARD_MESSAGE) { // Mensaje recibido para confirmar seteo de clase por jugador no lider
      char * message = client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

    }
    else if (msg_code == GET_NAME) { //Recibimos un mensaje inicial del servidor para un jugador cualquiera, se pide el nombre
      char * message = client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);
      printf("-> El lider ya fue asignado. \nIngrese nombre:\n");
      int option = 2;

      char * response = get_input();

      client_send_message(server_socket, option, response);
    }
    else if (msg_code == GET_NAME_LEADER) { // Mensaje recibido por el jugador lider y se le pide el nombre.
      char * message = client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

      printf("-> Ha sido asignado como lider de la sesión.\n-> Ingrese su nombre: \n");
      int option = 2;

      char * response = get_input();

      client_send_message(server_socket, option, response);
    }
    else if (msg_code == SELECT_SPELL) { // Se selecciona clase de cada jugador
      char * message = client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

      int option = 3;
      char * response;
      while(true){
        show_classes();
        printf("-> Seleccione su clase entre las opciones. \n");
        response = get_input();
        if ((strcmp(response, "0") == 0) || (strcmp(response, "1") == 0) || (strcmp(response, "2") == 0))
        {
          break;
        } else {
          printf("-> Seleccione una clase válida.\n");
        }
      }
      client_send_message(server_socket, option, response);
    }
    else if (msg_code == READY) { // Solo lo recibe el lider, para partir la partida
      char * message = client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

      printf("-> Listo para jugar?\n");
      int option = 4;

      char * response = get_input();

      client_send_message(server_socket, option, response);
    } else if (msg_code == ACTIVATE_PROMPT){ // Activa la comunicación del player con el servidor en su turno
      char * message = client_receive_payload(server_socket);
      printf("Es tu turno!\n\n");
      player_turn_watcher(server_socket);
    } 

    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
