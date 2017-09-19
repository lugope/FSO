#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 1984
#define clear() printf("\033[H\033[J")

void createSocket();
void enterInSocket(char* ip);
void* handleMessageSending(void* vargp);
void* handleMessageReading(void* vargp);
void* listenForNewSockets(void* vargp);
void dispatchThreads();
void menu();

int server_fd;
int socket_ids[100];
int self_socket;

char* own_nickname;

struct thr_args {
    struct sockaddr_in address;
    int address_length;
};

typedef struct thr_args thr_args;

struct rd_thr_args {
	int socket_id;
};

typedef struct rd_thr_args rd_thr_args;


int main(){

    while(1){
        menu();
    }

    return 0;
}

void menu(){

	printf("choose a nickname:\n");
	own_nickname = malloc(sizeof(char) * 50);
	scanf("%s",own_nickname);

    clear();

    printf("1 - Create a room\n");
    printf("2 - Enter in a room\n");
    printf("3 - exit\n");

    int option = 0;
    scanf("%d", &option);

    switch(option){
        case 1:
            createSocket();
            printf("setup ok!...\n");
            dispatchThreads();
            break;

        case 2:
            printf("Enter the ip:\n");

            char* str_ip = malloc(sizeof(char) * 20);
            scanf("%s",str_ip);

            enterInSocket(str_ip);
            dispatchThreads();
            break;

        default:
            exit(EXIT_SUCCESS);
            break;
    }

    clear();
}

void dispatchThreads(){
    pthread_t tid_sending;
    
    pthread_create(&tid_sending, NULL, handleMessageSending, (void *)1);
    pthread_exit(NULL);
}

void dispatchReadingThread(int socket_id){

	pthread_t tid_reading;

	rd_thr_args* arg = malloc(sizeof(rd_thr_args));
	arg->socket_id = socket_id;
			
	pthread_create(&tid_reading, NULL, handleMessageReading, (void *)arg);
}


void* handleMessageSending(void* vargp){

    char* message = malloc(sizeof(char) *100);

    while(1){
        // printf("\nSend message to client:\n");
        scanf("%[^\n]s", message);
		getchar();
		fflush(stdin);

        // printf("\nwaiting for client...\n");
		sleep(1);

		char* sending_message = malloc(sizeof(char) * 150);
		strcpy(sending_message, own_nickname);
		strcat(sending_message,": ");
		strcat(sending_message, message);		

		for(int i = 0; socket_ids[i] != 0; i++){
        	// write in socket

        	send(socket_ids[i] , sending_message , strlen(sending_message) , 0 );
		}
        // printf("message sent\n");
    }
}

void* handleMessageReading(void* vargp){
    

	int socket_id = ((rd_thr_args*) vargp)->socket_id;
    int valread;
    char buffer[1024] = {0};

    while(1){

        valread = read( socket_id , buffer, 1024);
        printf("%s\n", buffer);
		int len = strlen(buffer);
		
		for(int i = 0; socket_ids[i] != 0; i++){
			
			char* message = malloc(sizeof(char) * len);
			strcpy(message, buffer);
			if (socket_id == socket_ids[i] ){ continue; }
        	// printf("\nWating for new message...\n");
			send(socket_ids[i] , message, strlen(buffer) , 0);
		}
		
		memset(buffer, '\0', len);
		sleep(1);
    }
}

void* listenForNewSockets(void* vargp){

	thr_args args = *((thr_args*) (vargp));

	printf("There is no one in the room...\n");

	int i = 0;

	while(1){
    	int socket_id = accept(server_fd, (struct sockaddr*) &args.address, (socklen_t*) &args.address_length );

    	if(socket_id < 0){
        	perror("erro accept socket");
			printf("someone is trying to enter the room...\n");
    	}else{
			printf("\n** \tnew person entered the room\n** \t\tid:%d\n", socket_id);
			socket_ids[i] = socket_id;
			i++;
			socket_ids[i] = 0;

			dispatchReadingThread( socket_id);
		}
	}
}

void createSocket(){

	printf("setup socket...\n");

    int option = 1;
    struct sockaddr_in address;
    int address_length = sizeof(address);

	// printf("getting server fd...\n");
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0){
        perror("error getting socket");
        exit(EXIT_FAILURE);
    }


    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option) )){
        perror("error setting socket option");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //Binding socket to port
    if(bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0){
        perror("binding failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
        perror("error listening");
        exit(EXIT_FAILURE);
    }

	thr_args* args = malloc(sizeof(thr_args));
	(*args).address = address;
	(*args).address_length = address_length;

	pthread_t tid_listen;
	pthread_create(&tid_listen, NULL, listenForNewSockets, (void *)&args);
}

void enterInSocket(char* ip){
	
	printf("entering on %s...\n", ip);

    struct sockaddr_in address;
    int valread;
    struct sockaddr_in serv_addr;

    if ((socket_ids[0] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("error socket creation");
        exit(EXIT_FAILURE);
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)  {
        printf("\n-*-\nInvalid address/ Address not supported\n-*-\n");
        exit(EXIT_FAILURE);
    }
  
    if (connect(socket_ids[0], (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        exit(EXIT_FAILURE);
    }else{
	
		// read host messages
		dispatchReadingThread( socket_ids[0] );

		printf("you just entered in the room.\n");
	}
	
    
}
