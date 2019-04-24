/*
	cwait from cthread.h implementation.
*/

#include <stdlib.h>
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
			// Initializing iterator
			if (FirstFila2(sem->fila) == 0)
			{
				// Gets the current node
				TCB_t* currThread = (TCB_t*)sem->fila->it->node;

                // Check if resource is free
                // If count is <= 0 put thread on blocked state and on resource wait list 
                if (sem->count <=0)
                {
                    // Changes thread state to blocked
                    currThread->state = PROCST_BLOQ;
                    // Add thread to end of queue
                    AppendFila2(sem->fila, currThread);
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
		}
		else
		{
			// Null struct
			returnCode = OpNullStructError;
		}
	}

	return returnCode;
}