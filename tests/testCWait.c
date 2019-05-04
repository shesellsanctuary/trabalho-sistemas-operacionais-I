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
int dummyFunction()
{
	printf("dummy \n");
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
	ccreate((void*)dummyFunction, NULL, ThreadHighPriority);
	ccreate((void*)dummyFunction, NULL, ThreadHighPriority);
	// TCB_t* x = (TCB_t*)malloc(sizeof(TCB_t));
	// ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
	// getcontext(context);
	// x->context = *context;
	// x->prio = ThreadHighPriority;
	// x->state = PROCST_APTO;
	// x->tid = 1;

	// appendThreadToReadyQueue(x);

	// Remove main thread from execution
	cyield();
	
	TCB_t* thread1 = (TCB_t*)g_executingThread->first->node;
	printf("executing: %d\n", thread1->tid);

	if (cwait(pSem) != OpSuccess)
	{
		testStatus = TestError;
		printf("fail 1\n");
	}
	else
	{
		// First thread should be "executing", because it got the resource without waiting
		if (thread1->state != PROCST_EXEC)
		{
			TCB_t* thread = (TCB_t*)g_executingThread->first->node;
			printf("executing: %d\n", thread->tid);
			testStatus = TestError;
			printf("fail state 1\n state: %d\n tid: %d\n", thread1->state, thread1->tid);
		}
		// Creating new thread and appending to ready queue
		// TCB_t* y = (TCB_t*)malloc(sizeof(TCB_t));
		// ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
		// getcontext(context);
		// y->context = *context;
		// y->prio = ThreadHighPriority;
		// y->state = PROCST_APTO;
		// y->tid = 2;

		// appendThreadToReadyQueue(y);

		// Removing first thread from execution 
		cyield();
		TCB_t* thread2 = (TCB_t*)g_executingThread->first->node;
		printf("executing: %d\n", thread2->tid);
		
		// Requesting non existent resource
		if (cwait(pSem) != OpSuccess)
		{
			testStatus = TestError;
			printf("fail 2\n");
		}
		else
		{
			// Thread should be blocked and on both the resource and blocked queues
			if ((SearchThreadFila2(pSem->fila, thread2->tid) != OpSuccess) || 
				(thread2->state != PROCST_BLOQ) || 
				(SearchThreadFila2(g_blockedQueue, thread2->tid)) != OpSuccess)
			{
				printf("second thread not on sem list\n tid: %d\n state: %d\n", thread2->tid, thread2->state);
				testStatus = TestError;

			}
		}
		free(thread2);
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