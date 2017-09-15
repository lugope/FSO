#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//System import
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define TAMANHO_MEMORIA 100



// Arquivo servidor
int main(){
	
	int idMemoriaCompartilhada;
	key_t chave;
	char* memoriaCompartilhada;
	char* str;

	chave = 1984;

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

	memcpy(memoriaCompartilhada, "Hello FSO", 9);

	str = memoriaCompartilhada;
	str += 9;
	*str = 0;


	while(*memoriaCompartilhada != '*' ){
		sleep(1);
	}

	printf("Processo do cliente foi finalizado.\n");

	printf("\n... Adeus\n");

 
	return 0;
}

