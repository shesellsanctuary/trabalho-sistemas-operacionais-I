/*
	Here are the implementations of the scheduler, the queues for the threads and other necessary things
*/

#include <errno.h>
#include <stdlib.h>
#include "scheduler.h"
#include "EThreadPriority.h"

// Defining the variable
int g_numOfThreads = 0;

// Initialize the library
EOperationStatus initialize()
{
	// Return code
	EOperationStatus returnCode = OpSuccess;

	// Allocates queue
	g_readyQueues = (PFILA2)malloc(sizeof(FILA2));
	// Error
	if ((errno == ENOMEM) || (CreateFila2(g_readyQueues) != 0)) returnCode = OpAllocError;
	if (returnCode != OpAllocError)
	{
		// Creates each priority queue
		// High
		AppendFila2(g_readyQueues, malloc(sizeof(FILA2)));
		// Medium
		AppendFila2(g_readyQueues, malloc(sizeof(FILA2)));
		// Low
		AppendFila2(g_readyQueues, malloc(sizeof(FILA2)));
		
		// Allocates executing queue
		g_executingThread = (PFILA2)malloc(sizeof(FILA2));
		// Error
		if ((errno == ENOMEM) || (CreateFila2(g_executingThread) != 0)) returnCode = OpAllocError;
	}
	if (returnCode != OpAllocError)
	{
		// Allocates queue
		g_blockedQueue = (PFILA2)malloc(sizeof(FILA2));
		// Error
		if ((errno == ENOMEM) || (CreateFila2(g_blockedQueue) != 0)) returnCode = OpAllocError;
	}

	// Initializes tid count
	g_numOfThreads = 1;

	// TODO

	return returnCode;
}

// Appends to the right ready queue
EOperationStatus appendThreadToReadyQueue(TCB_t* threadToAppend)
{
	// Return code
	EOperationStatus returnCode = OpUknownError;

	// We should not add null to our list
	if (threadToAppend != NULL)
	{
		// Append return variable
		int appendStatus = -1;

		// Depending on the thread priority, we choose which queue to add it to
		switch (threadToAppend->prio)
		{
		case ThreadHighPriority:
			appendStatus = AppendFila2(g_HighPrioReadyQueue, threadToAppend);
			break;
		case ThreadMediumPriority:
			appendStatus = AppendFila2(g_MediumPrioReadyQueue, threadToAppend);
			break;
		case ThreadLowPriority:
			appendStatus = AppendFila2(g_LowPrioReadyQueue, threadToAppend);
			break;
		default:
			// Invalid priority received
			returnCode = OpInvalidPriorityError;
			break;
		}
		// If we added to queue correctly and append was successfull, operation done and success
		if ((returnCode != OpInvalidPriorityError) && (appendStatus == 0))
		{
			returnCode = OpSuccess;
		}
		else
		{
			returnCode = OpAppendError;
		}
	}
	else
	{
		returnCode = OpNullStructError;
	}

	return returnCode;
}