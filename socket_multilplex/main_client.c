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
  

int socket_id;

int main(){

    int sock = 0;
    int valread;
    char buffer[1024] = {0};

    socket_id = setupSocket();

    printf("Hearing from socket PORT 1984...\n");

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
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  {
        printf("\n-*-\nInvalid address/ Address not supported\n-*-\n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }
    
    return sock;
}
