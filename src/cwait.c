/*
	cwait from cthread.h implementation.
*/

#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
#include "EOperationStatus.h"

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
			if (sem->count <=0)
			{
				TCB_t* thread = (TCB_t*)g_executingThread->first->node;
				// Changes thread state to blocked
				thread->state = PROCST_BLOQ;
				
				// Add thread to end of queue
				AppendFila2(sem->fila, thread);
				returnCode = OpSuccess;
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
			// Null struct
			returnCode = OpNullStructError;
		}
	}

	return returnCode;
}