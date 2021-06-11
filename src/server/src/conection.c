
#include <pthread.h>
#include "conection.h"

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

void *myThreadFun(Thread_conectar * info_conectar)
{
  printf("Printing GeeksQuiz from Thread \n");
  struct sockaddr_in client1_addr;
  struct sockaddr_in client2_addr;
  struct sockaddr_in client3_addr;
  struct sockaddr_in client4_addr;
  struct sockaddr_in client5_addr;
  socklen_t addr_size = sizeof(client1_addr);

  char * welcome = "Bienvenido Cliente !!";
  

  printf("esperando clientes\n");
  info_conectar ->sockets_clients->socket_c1 = accept(info_conectar ->server_socket, (struct sockaddr *)&client1_addr, &addr_size);
  server_send_message(info_conectar ->sockets_clients->socket_c1, 3, welcome);
  printf("se conectó 1 cliente\n");
  info_conectar ->sockets_clients->socket_c2 = accept(info_conectar ->server_socket, (struct sockaddr *)&client2_addr, &addr_size);
  server_send_message(info_conectar ->sockets_clients->socket_c2, 2, welcome);
  printf("se conectó 1 cliente\n");
  info_conectar ->sockets_clients->socket_c3 = accept(info_conectar ->server_socket, (struct sockaddr *)&client3_addr, &addr_size);
  server_send_message(info_conectar ->sockets_clients->socket_c3, 2, welcome);
  printf("se conectó 1 cliente\n");
  info_conectar ->sockets_clients->socket_c4 = accept(info_conectar ->server_socket, (struct sockaddr *)&client4_addr, &addr_size);
  server_send_message(info_conectar ->sockets_clients->socket_c4, 2, welcome);
  printf("se conectó 1 cliente\n");
  info_conectar ->sockets_clients->socket_c5 = accept(info_conectar ->server_socket, (struct sockaddr *)&client5_addr, &addr_size);
  server_send_message(info_conectar ->sockets_clients->socket_c5, 2, welcome);
  printf("se conectó 1 cliente\n");
  return NULL;
}


PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port){
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



  PlayersInfo * sockets_clients = malloc(sizeof(PlayersInfo));
  Thread_conectar * info_conectar = malloc(sizeof(Thread_conectar));
  info_conectar->sockets_clients = sockets_clients;
  info_conectar->server_socket = server_socket;

  int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // Se coloca el socket en modo listening
  int ret3 = listen(server_socket, 1);

  pthread_t thread_id;
  printf("Before Thread\n");
  pthread_create(&thread_id, NULL, myThreadFun, info_conectar);
  //pthread_join(thread_id, NULL);

  printf("se retornó");
  return sockets_clients;
}
