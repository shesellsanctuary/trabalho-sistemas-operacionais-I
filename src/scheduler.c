/*
	Here are the implementations of the scheduler, the queues for the threads and other necessary things
*/

#include <errno.h>
#include <stdlib.h>
#include "scheduler.h"
#include "EThreadPriority.h"
#include "TBool.h"

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
	if ((errno == ENOMEM) || (CreateFila2(g_readyQueues) != 0))
		returnCode = OpAllocError;
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
		if ((errno == ENOMEM) || (CreateFila2(g_executingThread) != 0))
			returnCode = OpAllocError;
	}
	if (returnCode != OpAllocError)
	{
		// Allocates queue
		g_blockedQueue = (PFILA2)malloc(sizeof(FILA2));
		// Error
		if ((errno == ENOMEM) || (CreateFila2(g_blockedQueue) != 0))
			returnCode = OpAllocError;
	}

	// Initializes tid count
	g_numOfThreads = 1;

	// Initialize main thread
	//TCB_t mainThread;
	//mainThread.tid = MAIN_TID;
	//mainThread.context =

	// Call dispatcher for the main thread

	return returnCode;
}

// Appends to the right ready queue
EOperationStatus appendThreadToReadyQueue(TCB_t *threadToAppend)
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

// Dispatcher
// Move the first ready thread with the highest priority to the executing queue
// Should only be used after the queues have been initialized
// This function should not remove the executing thread, as it is non preemptive
// The user should swapcontext after its use
EOperationStatus dispatch()
{
	// Return code
	EOperationStatus returnCode = OpUknownError;
	// Queue which contains the thread
	PFILA2 readyQueue = NULL;

	// Get the first ready thread
	if (GetFirstReadyThread(&readyQueue) != OpUknownError)
	{
		// Add to executing queue and delete from ready queue
		if (AppendFila2(g_executingThread, readyQueue->it) == 0)
		{
			// Change thread state to executing, there should be only 1 here
			((TCB_t *)g_executingThread->first->node)->state = PROCST_EXEC;
			if (DeleteAtIteratorFila2(readyQueue) == 0)
			{
				// ucontext_t threadctxt = ((TCB_t *)g_executingThread->first->node)->context;
				// setcontext(threadctxt); or swapcontext?
				returnCode = OpSuccess;
			}
			else
			{
				// Unknown state, should we try to delete again?
				returnCode = OpDeleteError;
			}
		}
		else
		{
			// If this function fails, the program enters an unknown state, should we try to append again?
			returnCode = OpAppendError;
		}
	}

	return returnCode;
}

// Sets the iterator to the first available thread from the ready queue
EOperationStatus GetFirstReadyThread(PFILA2 *queueReference)
{
	// Return code
	EOperationStatus returnCode = OpUknownError;

	// Considers that the queues are already initialized
	if (((PFILA2)g_HighPrioReadyQueue)->first != NULL)
	{
		queueReference = g_HighPrioReadyQueue;
		(*queueReference)->it = ((PFILA2)g_HighPrioReadyQueue)->first;
		returnCode = OpSuccess;
	}
	else if (((PFILA2)g_MediumPrioReadyQueue)->first != NULL)
	{
		queueReference = g_MediumPrioReadyQueue;
		(*queueReference)->it = ((PFILA2)g_MediumPrioReadyQueue)->first;
		returnCode = OpSuccess;
	}
	else if (((PFILA2)g_LowPrioReadyQueue)->first != NULL)
	{
		queueReference = g_LowPrioReadyQueue;
		(*queueReference)->it = ((PFILA2)g_LowPrioReadyQueue)->first;
		returnCode = OpSuccess;
	}
	else
	{
		// There are no ready threads
		returnCode = OpNullStructError;
	}

	return returnCode;
}