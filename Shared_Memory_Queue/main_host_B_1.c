#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define SIZE 128




void quitProcess(char *s){
	perror(s);
	exit(1);
}

struct Msg_buf {
	long msg_type;
	char message[SIZE];
};

typedef struct Msg_buf Msg_buf;

int main(){

	printf("presenter -- terminal\n");

	int msg_queue_id;
	int msg_flag = 0666;
	key_t key;
	Msg_buf msg_buffer;

	int type = 1;

	key = 4343;
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
			printf("message:\n%s\n", msg_buffer.message);
		}

		sleep(1);
	}

	return 0;
}
