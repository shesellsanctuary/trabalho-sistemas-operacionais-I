/*
	csetprio from cthread.h implementation.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
#include "EThreadPriority.h"
#include "EOperationStatus.h"

int 
csetprio (int tid, int prio)
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
		if (prio == ThreadHighPriority || prio == ThreadMediumPriority || prio == ThreadLowPriority)
		{
			TCB_t* thread = (TCB_t*)g_executingThread->first->node;

			// Change thread priority
			thread->prio = prio;
			returnCode = OpSuccess;
		}
		else
		{
			returnCode = OpInvalidPrio;
		}
    }
    return returnCode;
}