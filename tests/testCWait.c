#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
#include "scheduler.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "testCSignal.h"
#include "EThreadPriority.h"

#include <stdio.h>

ETestStatus cwait_test()
{
	// Test status
	ETestStatus testStatus = TestSuccess;

	//  Pointer to semaphore
	// Allocates memory
	csem_t* pSem = (csem_t*)malloc(sizeof(csem_t));

	// Number of available slots for semaphore
	// Constant, do not change during execution of the test
	int c_count = 1;

	// No need to test return, because we assume all other functions are working
	csem_init(pSem, c_count);

	// Create 2 threads to compete for 1 resource
	TCB_t* x = (TCB_t*)malloc(sizeof(TCB_t));
	ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
	getcontext(context);
	x->context = *context;
	x->prio = ThreadHighPriority;
	x->state = PROCST_EXEC;
	x->tid = 1;
	
	if (AppendFila2(g_executingThread, x) != 0)
	{
		//nothing
	}
	if (cwait(pSem) != OpSuccess)
	{
		testStatus = TestError;
		printf("fail 1\n");
	}
	else
	{
		// Initializing executing queue iterator
		if (FirstFila2(g_executingThread) != 0)
		{
			printf("error initializing it\n");
		}
		// Deleting previous thread from queue
		if ( DeleteAtIteratorFila2(g_executingThread) != 0)
		{
			printf("error deleting 1\n");
		}

		// Creating new thread and appending to executing queue
		TCB_t* y = (TCB_t*)malloc(sizeof(TCB_t));
		ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
		getcontext(context);
		y->context = *context;
		y->prio = ThreadHighPriority;
		y->state = PROCST_EXEC;
		y->tid = 2;
		
		if (AppendFila2(g_executingThread, y) != 0)
		{
			printf("error appending 2\n");
		}
		// Requesting non existent resource
		if (cwait(pSem) != OpSuccess)
		{
			testStatus = TestError;
			printf("fail 2\n");
		}
		else
		{
			// First thread should be "executing", because it got the resource without waiting
			if (x->state != PROCST_EXEC)
			{
				testStatus = TestError;
				printf("fail state 1\n");
			}
			// Initializing sem iterator
			if (FirstFila2(pSem->fila) == 0)
			{
				TCB_t* firstThread = (TCB_t*)pSem->fila->it->node;
				// Second thread should be on sem list and with state blocked
				if ((firstThread->state != PROCST_BLOQ) && (testStatus != TestError))
				{
					testStatus = TestError;
				}
			}
		}
	free(y);
	}

	// !!!  IMPORTANT !!!
	// Needs to free the memory used always
	free(pSem);
	free(x);
	free(context);

	return testStatus;
}