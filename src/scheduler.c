/*
	Here are the implementations of the scheduler, the queues for the threads and other necessary things
*/

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"
#include "EThreadPriority.h"
#include "TBool.h"
#include "utils.h"

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
		PFILA2 tempQueue = (PFILA2)malloc(sizeof(FILA2));
		if (CreateFila2(tempQueue) == 0)
		{
			// High
			AppendFila2(g_readyQueues, tempQueue);
		}
		tempQueue = (PFILA2)malloc(sizeof(FILA2));
		if (CreateFila2(tempQueue) == 0)
		{
			// Medium
			AppendFila2(g_readyQueues, tempQueue);
		}
		tempQueue = (PFILA2)malloc(sizeof(FILA2));
		if (CreateFila2(tempQueue) == 0)
		{
			// Low
			AppendFila2(g_readyQueues, tempQueue);
		}

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
	if (returnCode != OpAllocError)
	{
		// Allocates queue
		g_cjoinQueue = (PFILA2)malloc(sizeof(FILA2));
		// Error
		if ((errno == ENOMEM) || (CreateFila2(g_cjoinQueue) != 0)) returnCode = OpAllocError;
	}
	if (returnCode != OpAllocError)
	{
		// Initializes tid count
		g_numOfThreads = 1;

		// Initialize the end of thread context
		getcontext(&endOfThreadContext);
		endOfThreadContext.uc_link = 0;
		endOfThreadContext.uc_stack.ss_sp = endOfThreadStack;
		endOfThreadContext.uc_stack.ss_size = STACK_SIZE;
		makecontext(&endOfThreadContext, (void(*)(void))threadEndFunction, 0);

		// Initialize main thread
		ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
		getcontext(context);

		TCB_t* thread = (TCB_t*)malloc(sizeof(TCB_t));
		thread->context = *context;
		thread->prio = ThreadLowPriority;
		thread->state = PROCST_APTO;
		thread->tid = MAIN_TID;
		// Add main thread to the ready list
		if (AppendFila2(g_LowPrioReadyQueue, thread) != 0)
		{
			perror("Error adding main thread to queue.");
			returnCode = OpAppendError;
		}

		// Call dispatcher for the main thread
		dispatch();
		// No need to set context
	}

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
	if (GetFirstReadyThread(&readyQueue) == OpSuccess)
	{
		// Add to executing queue and delete from ready queue
		if (AppendFila2(g_executingThread, readyQueue->it->node) == 0)
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
		(*queueReference) = g_HighPrioReadyQueue;
		(*queueReference)->it = ((PFILA2)g_HighPrioReadyQueue)->first;
		returnCode = OpSuccess;
	}
	else if (((PFILA2)g_MediumPrioReadyQueue)->first != NULL)
	{
		(*queueReference) = g_MediumPrioReadyQueue;
		(*queueReference)->it = ((PFILA2)g_MediumPrioReadyQueue)->first;
		returnCode = OpSuccess;
	}
	else if (((PFILA2)g_LowPrioReadyQueue)->first != NULL)
	{
		(*queueReference) = g_LowPrioReadyQueue;
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

// Unblocks the thread waiting for the current to end
// Assumes the queues have already been started
// Shuold return success if no thread were waited for, or found when searched in the blocked queue
EOperationStatus UnblockThreadWaitingForThis(int tidOfExecutingThread)
{
	// Return code
	EOperationStatus returnCode = OpSuccess;

	// Searches for the thread
	if ((SearchFila2(g_cjoinQueue, tidOfExecutingThread, 1) == OpSuccess) && (g_cjoinQueue->it != NULL))
	{
		// TID of thread is in the iterator of the queue, we should change its state and move it to the ready queue
		if ((SearchThreadFila2(g_blockedQueue, ((pair*)g_cjoinQueue->it->node)->p2) == OpSuccess) && (g_blockedQueue->it != NULL))
		{
			// Updates the state
			((TCB_t*)g_blockedQueue->it->node)->state = PROCST_APTO;

			// Adds to ready queue
			if (appendThreadToReadyQueue((TCB_t*)g_blockedQueue->it->node) != OpSuccess)
			{
				perror("Error adding thread to the ready queue.");
				returnCode = OpAppendError;
			}
			// Deletes from the blocked queue
			else if (DeleteAtIteratorFila2(g_blockedQueue) != OpSuccess)
			{
				perror("Error deleting thread from blocked queue.");
				returnCode = OpDeleteError;
			}
		}
	}

	return returnCode;
}

// Function that will be called once a thread ends, must do the necessary logic for the cjoin function
// This function never returns, as it just swaps the context to the next thread
// It will call the scheduler to call the next thread
// It will be inside a context, which the other threads need to link to
void threadEndFunction()
{
	// Searches the cjoin queue to unblock the threads that were waiting for the finished thread
	if (UnblockThreadWaitingForThis(((TCB_t*)g_executingThread->first->node)->tid) != OpSuccess)
	{
		perror("Error while trying to unblock thread.");
	}

	// Sets the iterator
	FirstFila2(g_executingThread);
	// Deletes the finished thread
	DeleteAtIteratorFila2(g_executingThread);

	// Puts the next thread into execution
	dispatch();

	// Set next executing thread context
	if (GetSizeFila2(g_executingThread) > 0)
	{
		setcontext(&((TCB_t*)g_executingThread->first->node)->context);
	}
}