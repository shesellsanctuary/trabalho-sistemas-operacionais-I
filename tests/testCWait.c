#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
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
	int c_count = 1000;

	// No need to test return, because we assume all other functions are working
	csem_init(pSem, c_count);

	// add to semaphore list
	int i = 0;
	for (; i < 10; i++)
	{
		TCB_t* x = (TCB_t*)malloc(sizeof(TCB_t));
		ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
		getcontext(context);
		x->context = *context;
		x->prio = ThreadHighPriority;
		x->state = PROCST_EXEC;
		x->tid = i;
		if (AppendFila2(pSem->fila, x) != 0)
		{
			// nothing
		}
	}
	// Test code

	if (csignal(pSem) != OpSuccess)
	{
		testStatus = TestError;
	}
	else
	{
		// Initializing iterator
		if (FirstFila2(pSem->fila) == 0)
		{
			// Check the thread queue for its state
			for (; pSem->fila->it != pSem->fila->last->next; NextFila2(pSem->fila))
			{
				// Gets the current node
				TCB_t* currThread = (TCB_t*)pSem->fila->it->node;
				// If the thread is in the right state
				// As only the first thread became ready, the others should stay in the same state
				if ((currThread->state != PROCST_EXEC) && (testStatus != TestError))
				{
					testStatus = TestError;
				}
			}
		}
	}

	// !!!  IMPORTANT !!!
	// Needs to free the memory used always
	free(pSem);

	return testStatus;
}