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

	int msg_queue_id;
	int msg_flag = IPC_CREAT | 0666;
	key_t key;
	Msg_buf s_buf;
	size_t buffer_len;

	key = 4242;
	msg_queue_id = msgget(key, msg_flag);
	
	if ( msg_queue_id < 0 ) {
		quitProcess("message queue id error");
	}

	s_buf.msg_type = 1;

	
	printf("Enter a message to send by queue:\n");

	scanf("%[^\n]", s_buf.message);

	getchar();

	buffer_len = strlen(s_buf.message);

	int sending_value = msgsnd(msg_queue_id, &s_buf, buffer_len, IPC_NOWAIT);

	if( sending_value < 0 ){
		printf("%d %ld %s, %lu\n", msg_queue_id, s_buf.msg_type, s_buf.message, buffer_len);

		quitProcess("message send error");
	} else {
		
		printf("message sent to queue\n");
	}



	return 0;
}
