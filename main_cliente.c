#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//System import
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define TAMANHO_MEMORIA 100



// Arquivo cliente
int main(){
	
	int idMemoriaCompartilhada;
	key_t chave;
	char* memoriaCompartilhada;
	char* str;

	chave = 1984;

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

	for(str = memoriaCompartilhada; *str != 0; str++ ){
		printf("%c", *str);
	}

	*memoriaCompartilhada = '*';

	printf("\n");

 
	return 0;
}

