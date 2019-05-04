/*
	cwait from cthread.h implementation.
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
	sem: ponteiro para uma variavel do tipo semaforo.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrario, retorna um valor negativo.
******************************************************************************/
int
cwait(csem_t *sem)
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
			// Check if resource is free
			// If count is <= 0 put thread on blocked state and on resource wait list 
			if (sem->count <= 0)
			{
				TCB_t* thread = (TCB_t*)g_executingThread->first->node;
				// Changes thread state to blocked
				thread->state = PROCST_BLOQ;

				// Starts executing iterator and deletes thread from executing queue
				if ((FirstFila2(g_executingThread) == OpSuccess) && (DeleteAtIteratorFila2(g_executingThread) == OpSuccess))
				{
					// First dispatch next thread to execution then adds the old thread to blocked queue and semaphore queue
					if ((dispatch() == OpSuccess) && 
						(AppendFila2(g_blockedQueue,thread) == OpSuccess) && 
						(AppendFila2(sem->fila, thread)) == OpSuccess)
					{	
						swapcontext(&thread->context, &((TCB_t*)g_executingThread->first->node)->context);
						returnCode = OpSuccess;
					}
					else
					{
						returnCode = OpAppendError;
					}
				}
				else
				{
					returnCode = OpDeleteError;
				}
				
				
			}
			else
			{
				// If it is free, put it to thread and continue
				// Request resource
				sem->count--;
				returnCode = OpSuccess;
			}
		}
		else
		{
			printf("sem = null\n");
			// Null struct
			returnCode = OpNullStructError;
		}
	}

	return returnCode;
}