#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 1984

int setupSocket();
void* handleMessageSending(void* vargp);
void* handleMessageReading(void* vargp);
void dispatchThreads();

int server_fd;
int socket_id;


int main(){

	printf("TCP Server in PORT 1984\n");

    socket_id = setupSocket();

    printf("setup ok!...\n");

    dispatchThreads();

    return 0;
}

void dispatchThreads(){
    pthread_t tid_sending;
    pthread_t tid_reading;
    
    pthread_create(&tid_sending, NULL, handleMessageSending, (void *)1);
    pthread_create(&tid_reading, NULL, handleMessageReading, (void *)2);
    pthread_exit(NULL);
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

        // write in socket
        send(socket_id , message , strlen(message) , 0 );
        // printf("message sent\n");
    }
}

void* handleMessageReading(void* vargp){

	char buffer[1024] = {0};
	int valread;
    
    while(1){
        // printf("\nWating for new message...\n");
        valread = read( socket_id , buffer, 1024);
        printf("other: %s\n",buffer );
		
		int len = strlen(buffer);
		memset(buffer, '\0', len);

		sleep(1);
    }
}

int setupSocket(){

	printf("setup socket...\n");

    int option = 1;
    struct sockaddr_in address;
    int address_length = sizeof(address);
    int new_socket_id;

	printf("getting server fd...\n");
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

	printf("waiting for client accept...\n");

    new_socket_id = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &address_length );

    if(new_socket_id < 0){
        perror("erro accept socket");
        exit(EXIT_FAILURE);
    }

    return new_socket_id;
}
