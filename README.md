
# Monster Hunter: Ruz


### Creadores:
 - Miguel Ortiz 17638119
 - Gabriel Faundez 16640373
 - Ignacia Baeza 16205200
 - Andrés Plass 16636287

## Instrucciones para ejecutar (Servidor y Cliente)
1) Para ejecutar el programa en la línea de comando se debe escribir: 
 - Para ejecutar el servidor: ./server -i 0.0.0.0 -p 8080
 - Para ejecutar un cliente: ./client -i 0.0.0.0 -p 8080
Esto hace que el server y el cliente se escuchen en el port 8080 y direccion ip 0.0.0.0. Nuestro servidor soporta hasta 4 clientes simultáneamente.


## Protocolo de comunicación
1) Luego de que se inicia el servidor se deben conectar los clientes. El servidor mandará un mensaje saludando a cada cliente que se conecta. Si el cliente es el lider se le dira que ha sido asignado como lider. Luego el servidor le pedirá el nombre a cada cliente.
    - Cuando el juego se inicia por primera vez el primer cliente que se conecta es asigando como lider, pero si se elige jugar de nuevo el lider es asignado al azar.

2) Luego de setear el nombre el servidor le pide a cada cliente que seleccione su clase.
3) Cada vez que un cliente elije nombre y clase, el líder recibe una notificación de la creacion de este jugador (Lo recibe una vez enviado un input, dado que el servidor espera un input del líder).
4) Luego de que todos los clientes han seleccionado su nombre y clase el lider puede empezar el juego.
    - El líder no podrá iniciar el juego antes de que todos los jugadores conectados hayan ingresado sus respectivos nombres y clases.

A continuación mostramos los mensajes que se mandan en el protocolo de comunicación.

Primero tenemos el protocolo de conexión. En este protocolo se crean 5 threads, uno para cada cliente, y el servidor escucha a los clientes simultáneamente. Esto sólo se utiliza para setear los nombres y clases de cada jugador.

### Lado del servidor
msg_code es la variable que recibe el servidor por medio de la función server_receive_id(socket).
- msg_code = 2: El servidor recibe el nombre del jugador y le envía un mensaje al cliente pidiendo su clase.
- msg_code = 3: El servidor recibe la clase del jugador. Si el jugador es el líder el servidor le pregunta al lider si esta todo listo, si no es el líder le envía un mensaje genérico y el cliente queda a la espera del líder.
- msg_code = 4: El servidor revisa si todos los jugadores han ingresado nombre y clase. Si esto no es así le envía al lider un mensaje diciendo que no todos los jugadores han seteado su nombre y clase por lo que el juego no puede iniciar. Si está todo listo se empieza el juego. 
- msg_code = RECEIVE_UUID: Recibe el uuid del obejtivo.
- msg_code = RECEIVE_SPELL: Recibe el spell seleccionado por el jugador.
- msg_code = SELECT_MONSTER: Recibe el mosntruo seleccionado por el jugador.
- msg_code = END_CONENCTION: Recibe que el jugador se desconecto.

aqui hay que poner que se matan los threads pero no se como quedo al final asi que eso falta

Luego de que se inicia el juego, se le pide al lider que elija un monstruo.

### Lado del cliente

Los códigos de mensaje recibidos son captados por el thread del cliente. Éste ejecuta acciones distintas dependiendo del mensaje que recibió. La lista de mensajes se define en src/common/communication.h en forma de *enum*, para preparar distintos mensajes.

- msg_code = NON_DEFINED: Este mensaje se activa cuando el msg_code es incorrecto o undefined.

- msg_code = STANDARD_MESSAGE: Envío de un mensaje genérico desde el servidor al cliente. Se imprime en pantalla del cliente el mensaje.

- msg_code = GET_NAME: Mensaje inicial del servidor, se le pide al cliente el nombre y luego de introducirlo, se le responde al servidor.

- msg_code = GET_NAME_LEADER: Parecido a GET_NAME, se pide el nombre del cliente pero este código es recibido solo por el líder. Además, se le avisa al cliente que es líder de la sesión.

- msg_code = SELECT_SPEC: Se le pide al cliente la clase a jugar. En consola se despliegan las distintas clases posibles a elección, donde se espera que el cliente ponga el índice de la clase. Acá se evalúa que el input sea correcto.

- msg_code = READY: Mensaje recibido solo por el líder. Se indica que está preparado y se evalúa si todos elijieron nombre y clase.

- msg_code = ACTIVATE_PROMPT: Este mensaje activa la interfaz gráfica para el cliente.

- msg_code = GET_ENTITIES: Este mensaje hace en se cree una entidad con la información de cada objetivo posible que el jugador puede atacar.

- msg_code = AVAILABLE_TARGET: Este mensaje entrega un target disponible para ser atacado.

- msg_code = PLAYER_TURN: Cuando el cliente recibe este mensaje se muestran en pantalla los objetivos posbiles para atacar.

- msg_code = SKIP_FASE: Este mensaje hace que el jugador se salte un turno.

- msg_code = GET_INFO: Este mensaje hace que se obtenga la información del jugador y se instancie un Player con esa ifnormación.

- msg_code = END_TURN: Este mensaje termina el turno de un jugador.

- msg_code = END_CONENCTION: Este mensaje desconecta a un jugador.

- msg_code = EVENT: Este mensaje envía desde el servidor lo que hizo otro jugador.

- msg_code = SELECT_MONSTER: Este mensaje le pide al jugador que seleccione un mosntruo.

- msg_code = GAME_MESSAGE: Este mensaje recibe un anuncio desde el servidor.

- msg_code = CONTINUE_PLAYING: Este mensaje pregunta si se quiere seguir jugando en otra partida.

## descripcion de los paquetes utilizados en la comunicacion entre cliente y servidor
### Paquete n°1
- pd_threads

## principales decisiones de diseño para construir el programa
- Para las conexiones iniciales de los clientes se crea un thread por cliente así el servidor los puede escuchar simultáneamente. Una vez que todos los jugadores ingresan su nombre y clase y el juego comienza, se dejan todos los threads en standby excepto uno y se juega sólo en ese thread. Si se inicia otra partida se vuelven a usar los threads de cada cliente para volver a ingresar las clases.



## Principales funciones del programa supuestos adicionales ocuparon,
### Principales funciones de servidor
- Conexion: Esta se encarga de la comuncación inicial con los clientes. En esta funcion se le pregunta el nombre y la clase a cada jugador y los guarda en la estructura información_thread. También le pide al lider que elija un monstruo y le pregunta si el juego esta listo para empezar.

- *Th_conectador: Esta función crea un thread para cada jugador que se conecta, para que así el servidor pueda escuchar a todos los jugadores simultáneamente. Se itera sobre los jugadores conectados y se crea un thread para cada uno y se le entrega la función Conexion.

- prepare_sockets_and_get_clients: Esta función crea y activa el socket del servidor poniendolo en modo listenting. Se crea un thread con los threads de los jugadores que están conectados y se escuchan todos simultáneamente.

- play_turn: Esta función simula un turno de un jugador.

- next_round: En esta función se juega una ronda del juego. Llama a a función play_turn() para cada jugador.




### Principales funciones del cliente
- main: El main controla lo que pasa cuando el cliente recibe cada msg_code. Dependiendo del mgs_code recibido el main le dice al cliente que es lo que tiene que hacer y le envia al servidor un mensaje si corresponde.

- get_target_info: Esta función crea una entidad con un objetivo que esta disponible pra ser atacado. Si el msg_code recibido es AVAILABLE_TARGET, el objetivo esta disponible y se retorna una entidad.A

- get_targets_info: Esta función busca todos los objetivos que estan disponibles para ser atacados y crea una entidad para cada uno con la función get_target_info()
