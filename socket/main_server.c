#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1984

int setupSocket();

int server_fd;


int main(){

	printf("TCP Server in PORT 1984\n");

    char buffer[1024] = {0};
    int value_read;
    int socket_id = setupSocket();

    char* message = malloc(sizeof(char) *100);

	printf("setup ok!...\n");

    while(1){
        printf("\nSend message to client:\n");
        scanf("%[^\n]s", message);
		getchar();
		fflush(stdin);

        printf("\nwaiting for client...\n");
		sleep(1);

        // write in socket
        send(socket_id , message , strlen(message) , 0 );
        printf("message sent\n");
    }

    return 0;
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
