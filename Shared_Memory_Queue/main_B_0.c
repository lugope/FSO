#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//System import
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define TAMANHO_MEMORIA 100
#define SIZE 128

// signatures
void setupSharedMemory(key_t chave);
void sendMessageToQueue(key_t key, char* message);
void readSharedMemory();

// global variables
char* memoriaCompartilhada;


//Queue message
struct Msg_buf {
	long msg_type;
	char message[SIZE];
};
typedef struct Msg_buf Msg_buf;

void quitProcess(char *s){
	perror(s);
	exit(1);
}


// Client side
//
// 
int main(){

	printf("client -- terminal\n");

	key_t sharedMemoryKey = 1984;
	setupSharedMemory(sharedMemoryKey);

	while(1){
		if (*memoriaCompartilhada == '*'){
			sleep(1);
			continue;
		}

		//read shared memory
		readSharedMemory();
	}
 
	return 0;
}

void setupSharedMemory(key_t chave){
	int idMemoriaCompartilhada;

	idMemoriaCompartilhada = shmget(chave, TAMANHO_MEMORIA, 0666);
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

void readSharedMemory(){
	
	char* str;
	char* message = malloc(sizeof(char) * 100);

	int i = 0;
	for(str = memoriaCompartilhada; *str != 0; str++ ){
		message[i] = *str;
		i++;
	}

	*memoriaCompartilhada = '*';

	key_t key = 4343; // id for second queue
	sendMessageToQueue(key, message);
}

void sendMessageToQueue(key_t key, char* message){

	printf("sending message: %s\n", message);

	int msg_queue_id;
	int msg_flag = IPC_CREAT | 0666;
	Msg_buf s_buf;
	size_t buffer_len;

	msg_queue_id = msgget(key, msg_flag);
	
	if ( msg_queue_id < 0 ) {
		quitProcess("message queue id error");
	}

	s_buf.msg_type = 1;
	strcpy(s_buf.message, message);

	buffer_len = strlen(s_buf.message);

	int sending_value = msgsnd(msg_queue_id, &s_buf, buffer_len, IPC_NOWAIT);

	if( sending_value < 0 ){
		printf("%d %ld %s, %lu\n", msg_queue_id, s_buf.msg_type, s_buf.message, buffer_len);
		quitProcess("message send error");
	} else {
		printf("message sent to queue\n");
	}
}
