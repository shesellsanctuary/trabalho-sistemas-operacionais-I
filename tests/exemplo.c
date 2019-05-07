
/*
 *	Programa de exemplo de uso da biblioteca cthread
 *
 *	Vers�o 1.0 - 14/04/2016
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 */
#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
#include "scheduler.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "EThreadPriority.h"
#include "utils.h"
#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

void* func0(void *arg) {
	printf("Eu sou a thread ID0 imprimindo \n");
	cwait((csem_t*)arg);
	printf("Peguei recurso e vou liberar a cpu\n");
	cyield();
	printf("Eu sou a thread ID0 depois do cyield\n");
	csignal((csem_t*)arg);
	return 0;
}

void* func1(void *arg) {
	printf("Eu sou a thread ID1 imprimindo\n");
	cwait((csem_t*)arg);
	printf("Tentei pegar recurso e devo estar bloqueada\n");
	return 0;
}

int mainExample() {

	int	id0, id1;

	// Pointer to semaphore
	// Allocates memory
	csem_t* pSem = (csem_t*)malloc(sizeof(csem_t));

	// Number of available slots for semaphore
	// Constant, do not change during execution of the test
	int c_count = 1;

	csem_init(pSem, c_count);

	id0 = ccreate(func0, (void *)&pSem, 1);
	id1 = ccreate(func1, (void *)&pSem, 1);

	printf("Eu sou a main ap�s a cria��o de ID0 e ID1\n");

	cjoin(id0);
	cjoin(id1);

	printf("Eu sou a main voltando para terminar o programa\n");
	return 0;
}
