#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 1984
#define clear() printf("\033[H\033[J")

int createSocket();
int enterInSocket(char* ip);
void* handleMessageSending(void* vargp);
void* handleMessageReading(void* vargp);
void dispatchThreads();
void menu();

int server_fd;
int socket_id;


int main(){

    while(1){
        menu();
    }

    return 0;
}

void menu(){

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
    
    int valread;
    char buffer[1024] = {0};

    while(1){
        // printf("\nWating for new message...\n");
        valread = read( socket_id , buffer, 1024);
        printf("other: %s\n",buffer );
		
		int len = strlen(buffer);
		memset(buffer, '\0', len);

		sleep(1);
    }
}

int createSocket(){

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

	printf("waiting for client accept...\n");

    socket_id = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &address_length );

    if(socket_id < 0){
        perror("erro accept socket");
        exit(EXIT_FAILURE);
    }
}

int enterInSocket(char* ip){
    struct sockaddr_in address;
    int sock = 0;
    int valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("error socket creation");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)  {
        printf("\n-*-\nInvalid address/ Address not supported\n-*-\n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }
    
    return sock;
}
