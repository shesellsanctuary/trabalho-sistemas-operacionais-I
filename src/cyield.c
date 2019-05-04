/*
	cyield from cthread.h implementation.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
#include "EOperationStatus.h"
#include "EThreadPriority.h"

/******************************************************************************
Parametros:
    void
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrario, retorna um valor negativo.
******************************************************************************/

int
cyield(void)
{
	// Operation status to be returned
	EOperationStatus returnCode = OpUknownError;
	// Initialization return code
	EOperationStatus initializationCode = OpSuccess;
	// Check if we need to initialize the library
	if (g_numOfThreads == 0)
	{
		initializationCode = initialize();
	}

	if (initializationCode == OpSuccess)
	{
		// Changes thread state from executing to ready
		TCB_t* thread = (TCB_t*)g_executingThread->first->node;
		thread->state = PROCST_APTO;

		// Starts executing iterator and deletes thread from executing queue
		if ((FirstFila2(g_executingThread) == OpSuccess) && (DeleteAtIteratorFila2(g_executingThread) == OpSuccess))
		{
			// First dispatch next thread to execution then adds the old thread to respective ready queue
			if ((dispatch() == OpSuccess) && (appendThreadToReadyQueue(thread) == OpSuccess))
			{	
				// Swap context
				swapcontext(&thread->context, &((TCB_t*)g_executingThread->first->node)->context);
				// Call dispatcher to put next thread on execution
				returnCode = OpSuccess;
			}
			// Else we just append to the ready queue and call the same thread again, because it is the only thread available
			else
			{
				if ((appendThreadToReadyQueue(thread) == OpSuccess) && (dispatch() == OpSuccess))
				{
					// Swap context
					swapcontext(&thread->context, &((TCB_t*)g_executingThread->first->node)->context);
					returnCode = OpSuccess;
				}
				else
				{
					returnCode = OpUknownError;
				}
			}
		}
		else
		{
			returnCode = OpDeleteError;
		}
	}

	return returnCode;
}