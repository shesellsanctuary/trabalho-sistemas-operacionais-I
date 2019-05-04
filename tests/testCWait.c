#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
#include "scheduler.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "EThreadPriority.h"
#include "utils.h"
#include <stdio.h>

int dummyFunction(csem_t* pSem)
{
	cwait(pSem);
	perror("Blocked thread resumed.\n");
	csignal(pSem);
	return 0;
}

ETestStatus cwait_test()
{
	// Initializes library
	initialize();

	// Test status
	ETestStatus testStatus = TestSuccess;

	// Pointer to semaphore
	// Allocates memory
	csem_t* pSem = (csem_t*)malloc(sizeof(csem_t));

	// Number of available slots for semaphore
	// Constant, do not change during execution of the test
	int c_count = 1;

	// No need to test return, because we assume all other functions are working
	csem_init(pSem, c_count);

	// Create 2 threads to compete for 1 resource
	ccreate((void*)dummyFunction, pSem, ThreadHighPriority);
	TCB_t* thread = ((PFILA2)g_HighPrioReadyQueue)->first->node;

	if (cwait(pSem) != OpSuccess)
	{
		testStatus = TestError;
		perror("Failed to get resource.\n");
	}
	else
	{
		// Lets the created thread run
		cyield();

		// First thread should be "executing", because it got the resource without waiting
		if (((TCB_t*)g_executingThread->first->node)->state != PROCST_EXEC)
		{
			testStatus = TestError;
			perror("Executing thread has wrong state.\n");
		}

		// Thread should be blocked and on both the resource and blocked queues
		if ((SearchThreadFila2(pSem->fila, thread->tid) != OpSuccess) ||
			(thread->state != PROCST_BLOQ) ||
			(SearchThreadFila2(g_blockedQueue, thread->tid)) != OpSuccess)
		{
			testStatus = TestError;
			perror("Thread not found on semaphore queue.\n");
		}
	}

	// !!!  IMPORTANT !!!
	// Needs to free the memory used always
	for (FirstFila2(pSem->fila); pSem->fila->it != pSem->fila->last->next; NextFila2(pSem->fila))
	{
		// Free elements of the queue
		free(pSem->fila->it->node);
	}
	free(pSem->fila);
	free(pSem);

	return testStatus;
}