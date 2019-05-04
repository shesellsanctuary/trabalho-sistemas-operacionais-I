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
		TCB_t* thread = (TCB_t*)g_executingThread->first->node;
		// Changes thread state from executing to ready
        thread->state = PROCST_APTO;
		// Adds thread to respective ready queue and starts executing iterator
		if ((appendThreadToReadyQueue(thread) != 0) || (FirstFila2(g_executingThread) != 0))
		{
			returnCode = OpUknownError;
		}
		else
		{
			printf("hiii\n");
			// Deleting thread from executing queue
			if (DeleteAtIteratorFila2(g_executingThread) == 0)
			{	// Call dispatcher to put next thread on execution
			printf("deleted thread from exec\n");
				// dispatch();
				returnCode = OpSuccess;
			}
			else
			{
				returnCode = OpDeleteError;
			}
		}
	}

	return returnCode;
}