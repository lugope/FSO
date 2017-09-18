#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//System import
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define TAMANHO_MEMORIA 100
#define SIZE 128 // Size of queue message

// void listenQueue(int incoming_key, void* listen_funcion(char*));
void listenQueue(int incoming_key, void (*listen_funcion)(char*));
void presentMessage(char* message);
char* copyString(char* string);
void setupSharedMemory(key_t memory_key);
void sendMessageToSharedMemory(char* string);



void quitProcess(char *s){
	perror(s);
	exit(1);
}

// Queue struct
struct Msg_buf {
	long msg_type;
	char message[SIZE];
};
typedef struct Msg_buf Msg_buf;

//variables to handle system state
char* memoriaCompartilhada;

// Server 
// 
int main(){

	printf("server -- terminal\n");

	// shared memory
	key_t key = 1984;
	setupSharedMemory(key);

	// listening queue
	key_t queue_key = 4242;
	listenQueue(queue_key, presentMessage);
	// in case it was heard, sent message to function: presentMessage
	// has while(true) forever without constraint, 
	// always listening from queue
 
	return 0;
}


//Initialize Queue listening
void listenQueue(int incoming_key, void (*listen_funcion)(char*)){
	
	int msg_queue_id;
	int msg_flag = 0666;
	key_t key = incoming_key;
	Msg_buf msg_buffer;

	int type = 1;

	//Get queue
	msg_queue_id = msgget(key, msg_flag);
	
	if ( msg_queue_id < 0 ) {
		quitProcess("message queue id error");
	}

	while(1){
		// Receive message from queue
		int message_isReceived = msgrcv(msg_queue_id, &msg_buffer, SIZE, type, 0 );

		if (message_isReceived < 0) {
			quitProcess("error receiving message");
		}else{
			(*listen_funcion)(msg_buffer.message);
		}

		sleep(1);
	}
}

void presentMessage(char* message){
	printf("message received by queue:\n%s\n", message);

	// Send message to shared memory
	sendMessageToSharedMemory(message);
}

char* copyString(char* string){

	int length = strlen(string);
	char* newStr = malloc(sizeof(char) * length + 1);
	strcpy(newStr, string);

	return newStr;
}

void setupSharedMemory(key_t chave){

	int idMemoriaCompartilhada;

	idMemoriaCompartilhada = shmget(chave, TAMANHO_MEMORIA, IPC_CREAT | 0666);
	if ( idMemoriaCompartilhada < 0 ){
		perror("shmget error");
		exit(1);
	}

	memoriaCompartilhada = shmat(idMemoriaCompartilhada, NULL, 0);
	
	if ( memoriaCompartilhada == (char*) -1 ){
		perror("shmat error");
		exit(1);
	}

}

void sendMessageToSharedMemory(char* string){

	printf("sending message to shared memory...");

	char* message = copyString(string);
	int length = strlen(message) + 1;
	memcpy(memoriaCompartilhada, message, length);
	
	// pointer to end of message
	memoriaCompartilhada += length;
	*memoriaCompartilhada = 0; // setup ending message char

	printf("waiting for the message is readed...");
	//Wait for the message is readed
	while(*memoriaCompartilhada != '*' ){
		sleep(1);
	}

	printf("message readed...");
	
}