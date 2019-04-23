/*
	csignal from cthread.h implementation.
*/

#include <stdlib.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
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
			// Initializing iterator
			if (FirstFila2(sem->fila) == 0)
			{
				// Gets the current node
				TCB_t* currThread = (TCB_t*)sem->fila->it->node;
				// Free resource
				sem->count++;
				// Changes thread state to ready
				currThread->state = PROCST_APTO;

				// TODO change from blocked list to ready list
				
				// Search in the blocked queue
				// If found, move to the ready queue
				// Else all is fine (sortof)

				// Remove the first element
				if (DeleteAtIteratorFila2(sem->fila) != 0)
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
		}
		else
		{
			// Null struct
			returnCode = OpNullStructError;
		}
	}

	return returnCode;
}