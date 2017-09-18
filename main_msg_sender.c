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

	printf("scanner -- terminal\n");

    sleep(1);

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

	fflush(stdin);
	
	char* str = malloc(sizeof(char) * 2);

	printf("enter 1 to begin:\n");

	while(1){

		scanf("%s",str);

		if(str[0] == '1'){
			break;
		}
	}

	while(1){

		printf("Enter a message to send by queue: \ntype 0 to leave \n");

		scanf("%s[^/n]", s_buf.message);
		
		
		printf("%s\n", s_buf.message);

		printf("%c %d\n", s_buf.message, s_buf.message);
		if (s_buf.message[0] == '0'){
			break;
		}

		getchar();

		buffer_len = strlen(s_buf.message);

		int sending_value = msgsnd(msg_queue_id, &s_buf, buffer_len, IPC_NOWAIT);

		if( sending_value < 0 ){
			printf("%d %ld %s, %lu\n", msg_queue_id, s_buf.msg_type, s_buf.message, buffer_len);

			quitProcess("message send error");
		} else {
			printf("message sent to queue\n");
		}

		sleep(1);
	}


	return 0;
}
