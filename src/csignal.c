/*
	csignal from cthread.h implementation.
*/

#include <stdlib.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
#include "utils.h"
#include "EOperationStatus.h"

// Number of threads variable
extern int g_numOfThreads;

/******************************************************************************
Parâmetros:
	sem:	ponteiro para uma variável do tipo semáforo.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int
csignal(csem_t *sem)
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
		// If allocation was successfull
		if (sem != NULL)
		{
			// Free resource
			sem->count++;
			// Initializing iterator
			if (FirstFila2(sem->fila) == 0)
			{
				// Gets the current node
				TCB_t* currThread = (TCB_t*)sem->fila->it->node;
				// Changes thread state to ready
				currThread->state = PROCST_APTO;

				// Move the thread between queues
				// If search operation was successfull and found the thread in the queue
				if ((SearchThreadFila2(g_blockedQueue, currThread->tid) == OpSuccess) &&
					(g_blockedQueue->it != NULL))
				{
					// Remove from the blocked queue
					DeleteAtIteratorFila2(g_blockedQueue);
					// Insert in the ready queue
					returnCode = appendThreadToReadyQueue(currThread);
				}

				// Remove the first element
				if ((returnCode == OpSuccess) && (DeleteAtIteratorFila2(sem->fila) != 0))
				{
					// Delete error
					returnCode = OpDeleteError;
				}
				else
				{
					// Updates return code
					returnCode = OpSuccess;
				}
			}
			// Couldnt position to first
			else
			{
				returnCode = OpUknownError;
			}
		}
		else
		{
			// Null struct
			returnCode = OpNullStructError;
		}
	}

	return returnCode;
}