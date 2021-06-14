#include "conection.h"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "game/gameboard.h"

//LINKS REFERENCIAS:
//https://www.man7.org/linux/man-pages/man2/socket.2.html
//https://man7.org/linux/man-pages/man7/socket.7.html
//https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html
//https://man7.org/linux/man-pages/man2/setsockopt.2.html
//https://linux.die.net/man/3/htons
//https://linux.die.net/man/3/inet_aton
//https://www.howtogeek.com/225487/what-is-the-difference-between-127.0.0.1-and-0.0.0.0/
//https://www.man7.org/linux/man-pages/man2/bind.2.html
//https://www.man7.org/linux/man-pages/man2/accept.2.html

int id_threads = 0;

char * revert(char * message){
  //Se invierte el mensaje

  int len = strlen(message) + 1;
  char * response = malloc(len);

  for (int i = 0; i < len-1; i++)
  {
    response[i] = message[len-2-i];
  }
  response[len-1] = '\0';
  return message;
}


void *Th_conectador(Informacion_conectar * info_conectar)
{
  struct sockaddr_in client_addr[5];
  socklen_t addr_size = sizeof(client_addr[0]);
  char * welcome = "Bienvenido Cliente !!";
  
  for (int i = 0; i<5;i++){ //Seteamos ninguno conectado
    info_conectar->conexiones[i] = false;
  }
  printf("Esperando clientes\n");
  
  for (int i = 0; i<5; i++){
  info_conectar->sockets_clients[i] = accept(info_conectar->server_socket, (struct sockaddr *)&client_addr[i], &addr_size);
  if (i == 0)
  { // Si es el lider recibe msjs distintos
    server_send_message(info_conectar ->sockets_clients[i], 3, welcome);
  } else { // Jugador normal
    server_send_message(info_conectar ->sockets_clients[i], 2, welcome);
  }
  printf("se conectó un cliente de índice %d\n", i);
  info_conectar->conexiones[i] = true;
  }
  return NULL;
}


Informacion_conectar * prepare_sockets_and_get_clients(char * IP, int port){
  // Se define la estructura para almacenar info del socket del servidor al momento de su creación
  struct sockaddr_in server_addr;

  // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
  int opt = 1;
  int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  // Se guardan el puerto e IP en la estructura antes definida
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(IP, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);

  Informacion_conectar * info_conectar = malloc(sizeof(Informacion_conectar));
  info_conectar->server_socket = server_socket;
  int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  // Se coloca el socket en modo listening
  int ret3 = listen(server_socket, 1);
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, Th_conectador, info_conectar);
  //pthread_join(thread_id, NULL);
  return info_conectar;
}

//Acá está la comunicacion directa a cada cliente
void *Conexion(Informacion_juego * informacion_thread)
{

  int my_attention = id_threads;
  id_threads += 1;
  Player **player_list = informacion_thread->jugadores;
  Player *player = spawn_player();
  int socket;

  while (1)
  {
    socket = informacion_thread->informacion_conexiones->sockets_clients[my_attention];
    int msg_code = server_receive_id(socket);
    
    if (msg_code == 2) //Recepción del nombre, son todos validos
    {
      char * client_message = server_receive_payload(socket);
      printf("-> El cliente %d seteó su nombre como: %s\n", my_attention+1, client_message);

      player_list[my_attention]->name = client_message;
      char * response = "Se seteó su nombre en el servidor";

      server_send_message(socket, 4, response);
    }
    else if (msg_code == 3) //Recepción de la clase, se validó en cliente
    {
      char * client_message = server_receive_payload(socket);
      printf("-> El cliente %d eligio la clase: %s\n", my_attention+1, client_message);

      set_player_class(player_list[my_attention], atoi(client_message));
      char * response = "Se seteó su clase en el servidor";

      if (my_attention == 0) // Si es que es lider, le mandamos una respuesta
      {
        server_send_message(socket, 5, response);
      } else {
        server_send_message(socket, 1, response);
      }
    }
    else if (msg_code == 4) // Revision de que todos hayan elegido nombre y clase para comenzar
    {
      char * client_message = server_receive_payload(socket);
      printf("-> El lider quiere partir la partida, se revisa si todos han elegido nombre\n");
      bool listo = true;
      char * response = revert(client_message);
      for (int i = 0;i<PLAYER_NUMBER;i++){
        
        if(informacion_thread->informacion_conexiones->conexiones[i]){
          printf("-> El cliente %d está conectado\n", i);
          //ver si hay nombre y clase elegido en esa misma posición
          //printf("pos %d; nombre %s; clase %s\n",i, player_list[i]->name, get_class_name(player_list[i]->spec));
          if (player_list[i]->name == NULL){
            listo = false;
            printf("-> El cliente %d no está listo\n", i);
            break;
          }
          if (player_list[i]->spec == -1){
            listo = false;
            printf("-> El cliente %d no está listo\n", i);
            break;
          }
          
        }
      }

      if (listo)
      {
        printf("-> Todo listo\n");
        //Parte la partida
        next_round(informacion_thread, informacion_thread->informacion_conexiones);


      } else 
      { //Se reenvia pregunta al lider
        char * response = "Algun jugador no esta listo";
        server_send_message(socket, 5, response);
      }
    }
  }
  
  return NULL;
}