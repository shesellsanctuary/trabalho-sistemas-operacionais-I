#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
#include "scheduler.h"
#include "utils.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "testCSignal.h"
#include "EThreadPriority.h"

ETestStatus csignal_test()
{
	// Initializes library
	initialize();

	// Test status
	ETestStatus testStatus = TestSuccess;

	//  Pointer to semaphore
	// Allocates memory
	csem_t* pSem = (csem_t*)malloc(sizeof(csem_t));

	// Number of available slots for semaphore
	// Constant, do not change during execution of the test
	int c_count = 1000;

	// Number of threads created for this test
	int cNumOfThreads = 100;

	// No need to test return, because we assume all other functions are working
	csem_init(pSem, c_count);

	// add to semaphore list
	int i = 0;
	// Set context
	ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
	getcontext(context);

	for (; i < cNumOfThreads; i++)
	{
		TCB_t* thread = (TCB_t*)malloc(sizeof(TCB_t));
		thread->context = *context;
		thread->prio = ThreadHighPriority;
		thread->state = PROCST_BLOQ;
		thread->tid = i;
		AppendFila2(g_blockedQueue, thread);
		AppendFila2(pSem->fila, thread);
	}

	// Issues two signals
	if ((csignal(pSem) != OpSuccess) || (csignal(pSem) != OpSuccess))
	{
		testStatus = TestError;
	}
	else
	{
		// Initializing iterator
		if (FirstFila2(pSem->fila) == 0)
		{
			// Check the semaphore thread queue for its state
			for (; (pSem->fila->it != pSem->fila->last->next) && (testStatus != TestError); NextFila2(pSem->fila))
			{
				// Gets the current node
				TCB_t* currThread = (TCB_t*)pSem->fila->it->node;
				// If the thread is in the right state
				// As only the first thread became ready, the others should stay in the same state
				if ((currThread->state != PROCST_BLOQ) && (testStatus != TestError))
				{
					testStatus = TestError;
				}
				// Check the blocked queue, all of the threads should be on it, if not, error
				if (!((SearchThreadFila2(g_blockedQueue, currThread->tid) == OpSuccess) &&
					(g_blockedQueue->it != NULL)))
				{
					testStatus = TestError;
				}
			}
		}
		// We should have one thread in the ready queue and the other 9 blocked
		if ((testStatus != TestError) && 
			((GetSizeFila2(g_blockedQueue) != (cNumOfThreads - 2)) || (GetSizeFila2((PFILA2)g_HighPrioReadyQueue) != 2)))
		{ 
			testStatus = TestError;
		}
	}

	// !!!  IMPORTANT !!!
	// Needs to free the memory used always
	// For each thread in the semaphore queue free itself
	while(pSem->fila->it != pSem->fila->last->next)
	{
		FirstFila2(pSem->fila);
		free(pSem->fila->it->node);
		DeleteAtIteratorFila2(pSem->fila);
	}
	// Free queue
	free(pSem->fila);

	// Free semaphore
	free(pSem);

	// Free context
	free(context);

	return testStatus;
}