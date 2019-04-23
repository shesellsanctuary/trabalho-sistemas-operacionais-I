/*
	Here are the implementations of the scheduler, the queues for the threads and other necessary things
*/

#include <errno.h>
#include <stdlib.h>
#include "scheduler.h"

// Defining the variable
int g_numOfThreads = 0;

// Initialize the library
EOperationStatus initialize()
{
	// Return code
	EOperationStatus returnCode = OpSuccess;

	// Allocates queue
	g_readyQueue = (PFILA2)malloc(sizeof(FILA2));
	// Error
	if ((errno == ENOMEM) || (CreateFila2(g_readyQueue) != 0)) returnCode = OpAllocError;
	if (returnCode != OpAllocError)
	{
		// Allocates queue
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