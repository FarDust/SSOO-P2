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


void *Th_conectador(Informacion_juego * info_juego)
{
  struct sockaddr_in client_addr[5];
  socklen_t addr_size = sizeof(client_addr[0]);
  char * welcome = "Bienvenido Cliente !!";
  
  for (int i = 0; i<5;i++){ //Seteamos ninguno conectado
    info_juego->respondieron[i] = false;
    info_juego->informacion_conexiones->conexiones[i] = false;
  }
  printf("Esperando clientes\n");
  
  for (int i = 0; i<5; i++){
    info_juego->informacion_conexiones->sockets_clients[i] = accept(info_juego->informacion_conexiones->server_socket, (struct sockaddr *)&client_addr[i], &addr_size);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, Conexion, info_juego);
    info_juego->informacion_conexiones->escuchadores[i] = thread_id;

    if (i == 0)
    { // Si es el lider recibe mensajes distintos
      server_send_message(info_juego->informacion_conexiones ->sockets_clients[i], 3, welcome);
    } else { // Jugador normal
      server_send_message(info_juego->informacion_conexiones ->sockets_clients[i], 2, welcome);
    }
    printf("se conectó un cliente de índice %d\n", i);
    info_juego->informacion_conexiones->conexiones[i] = true;
  }
  return NULL;
}


Informacion_juego * prepare_sockets_and_get_clients(char * IP, int port){
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

  Informacion_juego* informacion_thread = malloc(sizeof(Informacion_juego));
  informacion_thread->jugadores = get_player_list();
  informacion_thread->status = malloc(sizeof(GameStatus));
  informacion_thread->status->players = get_player_list();

  informacion_thread->informacion_conexiones = malloc(sizeof(Informacion_conectar));
  informacion_thread->informacion_conexiones->server_socket = server_socket;

  for (size_t i = 0; i < PLAYER_NUMBER; i++)
  {
    informacion_thread->continue_playing[i] = false;
  }
  

  int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  // Se coloca el socket en modo listening
  int ret3 = listen(server_socket, 1);
  pthread_t thread_id;
  
  pthread_create(&thread_id, NULL, Th_conectador, informacion_thread);

  informacion_thread->informacion_conexiones->main_connector = thread_id;
  return informacion_thread;
}

//Acá está la comunicacion directa a cada cliente
void *Conexion(Informacion_juego * informacion_thread)
{

  int my_attention = id_threads;
  id_threads += 1;
  Player **player_list = informacion_thread->status->players;
  Player *player = spawn_player();
  Monster *monster;
  informacion_thread->continue_playing[player->index] = true;
  int socket;

  while (1)
  {
    socket = informacion_thread->informacion_conexiones->sockets_clients[my_attention];
    if (!informacion_thread->ready){
      int msg_code = server_receive_id(socket);
      
      if (msg_code == 2) //Recepción del nombre, son todos validos
      {
        char * client_message = server_receive_payload(socket);
        printf("-> El cliente %d seteó su nombre como: %s\n", my_attention+1, client_message);

        player_list[my_attention]->properties->name = client_message;
        char * response = "Se seteó su nombre en el servidor";
        server_send_message(socket, 4, response);
      }
      else if (msg_code == 3) //Recepción de la clase, se validó en cliente
      {
        char * client_message = server_receive_payload(socket);
        printf("-> El cliente %d eligio la clase: %s\n", my_attention+1, client_message);

        set_player_class(player_list[my_attention], atoi(client_message));
        free(client_message);
        char * response = "Se seteó su clase en el servidor";

        if (my_attention == LEADER) // Si es que es lider, le mandamos una respuesta
        {
          server_send_message(socket, 5, response);
        } else {
          server_send_message(socket, 1, response);
          //mandar mensaje al lider con nombre y clase de nuevo jugador
          int leader_socket = informacion_thread->informacion_conexiones->sockets_clients[LEADER];
          char * message = (char*)malloc(64 * sizeof(char));
          sprintf(message, "el cliente %d seteó su nombre %s y clase %d", my_attention, player_list[my_attention]->properties->name, player_list[my_attention]->spec);
          server_send_message(leader_socket, STANDARD_MESSAGE, message);
          free(message);

        }
      }
      else if (msg_code == 4) // Revision de que todos hayan elegido nombre y clase para comenzar
      {
        for (int i = 0; i < PLAYER_NUMBER; i++)
        {
          informacion_thread->respondieron[i] = false;
        }
        
        char * client_message = server_receive_payload(socket);
        printf("-> El lider quiere partir la partida, se revisa si todos han elegido nombre\n");
        bool listo = true;

        for (int i = 0;i<PLAYER_NUMBER;i++){
          
          if(informacion_thread->informacion_conexiones->conexiones[i]){
            printf("-> El cliente %d está conectado\n", i);
            //ver si hay nombre y clase elegido en esa misma posición
            //printf("pos %d; nombre %s; clase %s\n",i, player_list[i]->name, get_class_name(player_list[i]->spec));
            if (player_list[i]->properties->name == NULL){
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

        free(client_message);

        if (listo)
        {
          printf("-> Todo listo\n");
          //Parte la partida


          //seteamos la partida como lista
          //matamos el thread que estaba escuchando
          server_send_message(socket, SELECT_MONSTER, "Selecciona un monstruo:\n0.- Great JagRuz\n1.- Ruzalos\n2.- Ruiz\n");
          bool seguir = true;
          while (seguir)
          {
            msg_code = server_receive_id(socket);
            if (msg_code==SELECT_MONSTER){
              char* client_monster_message = server_receive_payload(socket);
              if (informacion_thread->status->monster == NULL){
                monster = spawn_monster(atoi(client_monster_message));
              }else {
                monster = set_monster(informacion_thread->status->monster, atoi(client_monster_message));
              }
              informacion_thread->status->monster = monster;
              seguir = false;
              free(client_monster_message);
            } else if (msg_code == 0)
            {
              break;
            } else 
            {
              char* client_monster_message = server_receive_payload(socket);
              printf("[Client]: Mensaje sin sentido recibido\nmsg_code: %d\nmessage: %s\n", msg_code, client_monster_message);
              free(client_monster_message);
              break;
            }
          }
          informacion_thread->ready = true;
        } else 
        { //Se reenvia pregunta al lider
          char * response = "Algun jugador no esta listo";
          server_send_message(socket, 5, response);
        }
      } else if (msg_code == CONTINUE_PLAYING){
        char * client_message = server_receive_payload(socket);
        int response = atoi(client_message);
        free(client_message);
        informacion_thread->respondieron[my_attention] = true;
        if (response == 1){
          char * response = "Se seteó su nombre en el servidor";
          player->properties->health = player->properties->max_health;
          for (size_t buff = 0; buff < MAX_BUFFS; buff++)
          {
            player->properties->buff[buff] = 0;
          }
          server_send_message(socket, SELECT_SPEC, response);
        } else {
          
          informacion_thread->informacion_conexiones->conexiones[my_attention] = false;

          // se setea un nuevo LEADER
          if (my_attention == LEADER)
          {
          while(1){
            time_t t;
            srand((unsigned) time(&t));
            size_t NEW_LEADER= rand() % 6; //generar numero random entre lower y upper

            if (informacion_thread->informacion_conexiones->conexiones[NEW_LEADER]) // si está conectado aun
            {
              LEADER = NEW_LEADER;
              printf("[Server] Nuevo lider %ld", LEADER);
              break;
            }
            
            
          }

          }

          player->properties->health = 0;
          informacion_thread->continue_playing[player->index] = false;
          server_send_message(socket, END_CONENCTION, "Cerrando connexion al tablero\n");
          break;
        }
        
      } else if (msg_code != 0){
        char * client_message = server_receive_payload(socket);
        printf("-> Paquete sin sentido encontrado con \nmsg_code: %d\n message: %s\n",msg_code, client_message);
        free(client_message);
      }
    }

  }
  
  return NULL;
}