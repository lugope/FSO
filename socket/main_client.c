#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1984

int setupSocket();
  
int main(){

    int sock = 0;
    int valread;
    char buffer[1024] = {0};

    sock = setupSocket();

    printf("Hearing from socket PORT 1984...\n");

    while(1){
        printf("\nWating for new message...\n")
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
    }

    return 0;
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