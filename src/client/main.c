#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"
#include "player/prompts.h"
#include "utility/utility.h"
#include "gameboard/gameboard.h"



int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = argv[2]; // 0.0.0.0
  int PORT = atoi(argv[4]); // 8080

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);
  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto

  bool connected = true;
  while (connected){
    int msg_code = client_receive_id(server_socket);

    if (msg_code == STANDARD_MESSAGE) { // Mensaje recibido para confirmar seteo de clase por jugador no lider
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

    } else if (msg_code == GAME_MESSAGE) { // Mensaje recibido para confirmar seteo de clase por jugador no lider
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

    } else if (msg_code == GET_NAME) { //Recibimos un mensaje inicial del servidor para un jugador cualquiera, se pide el nombre
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);
      printf("-> El lider ya fue asignado. \nIngrese nombre:\n");
      int option = 2;

      char * response = get_input();

      client_send_message(server_socket, option, response);
      free(response);
    }
    else if (msg_code == GET_NAME_LEADER) { // Mensaje recibido por el jugador lider y se le pide el nombre.
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

      printf("-> Ha sido asignado como lider de la sesión.\n-> Ingrese su nombre: \n");
      int option = 2;

      char * response = get_input();

      client_send_message(server_socket, option, response);
      free(response);
    }
    else if (msg_code == SELECT_SPEC) { // Se selecciona clase de cada jugador
      char * message = (char *)client_receive_payload(server_socket);
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
          free(response);
        }
      }
      client_send_message(server_socket, option, response);
      free(response);
    }
    else if (msg_code == READY) { // Solo lo recibe el lider, para partir la partida
      char * message = (char *)client_receive_payload(server_socket);
      printf("-> El servidor dice: %s\n", message);
      free(message);

      printf("-> Listo para jugar?\n");
      int option = 4;

      char * response = get_input();

      client_send_message(server_socket, option, response);

      free(response);
    } else if (msg_code == ACTIVATE_PROMPT){ // Activa la comunicación del player con el servidor en su turno
      char * message = (char *)client_receive_payload(server_socket);
      printf("[Server]: %s", message);
      printf("Es tu turno!\n\n");
      free(message);
      player_turn_watcher(server_socket);
    } else if (msg_code == END_TURN){
      char * message = (char *)client_receive_payload(server_socket);
      printf("[Server]: %s", message);
      free(message);
      printf("------------------\n");
    } else if (msg_code == END_CONENCTION){
      char * message = (char *)client_receive_payload(server_socket);
      printf("[Server]: %s", message);
      free(message);
      char return_message[] = "[Client]: Conexión terminada correctamente\n";
      client_send_message(server_socket, END_CONENCTION, return_message);
      printf("%s", return_message);
      connected = false;
    }else if (msg_code == EVENT) {
      char * message = (char *)client_receive_payload(server_socket);
      printf("[EVENT] %s\n", message);
      free(message);
    } else if (msg_code == SELECT_MONSTER){
      char * message = (char *)client_receive_payload(server_socket);
      printf("%s\n", message);
      char * response = get_input();
      client_send_message(server_socket, SELECT_MONSTER, response);
      free(message);
      free(response);
    } else if (msg_code == CONTINUE_PLAYING){
      char * message = (char *)client_receive_payload(server_socket);
      printf("[Server]: %s\n", message);
      free(message);

      printf("otro - no\n");
      printf("1 - si\n");

      printf("Quieres continuar jugando?\n");
      char * response = get_input();

      client_send_message(server_socket, CONTINUE_PLAYING, response);
      free(response);
    }else if (msg_code == 0){
      connected = false;
    }
  }

  // Se cierra el socket
  close(server_socket);
  //free(IP);

  return 0;
}
