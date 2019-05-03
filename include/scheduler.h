#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <ucontext.h>

#include "support.h"
#include "cdata.h"
#include "EOperationStatus.h"

#define MAIN_TID 0
// Each of the ready queues sub queue
#define g_HighPrioReadyQueue g_readyQueues->first->node
#define g_MediumPrioReadyQueue g_readyQueues->first->next->node
#define g_LowPrioReadyQueue g_readyQueues->first->next->next->node

// End of thread execution context
ucontext_t endOfThreadContext;

// Stack for the end of thread
char endOfThreadStack[STACK_SIZE];

// Queues with the ready threads, one for each priority
PFILA2 g_readyQueues;

// Blocked threads
PFILA2 g_blockedQueue;

// Executing thread
PFILA2 g_executingThread;

// Queues with the with a pair of the thread blocked by cjoin and the thread it is waiting for
PFILA2 g_cjoinQueue;

// Number of total threads created since the beginning 
int g_numOfThreads;

// Initialize the library and its queues and necessary stuff
EOperationStatus initialize();

// Appends to the right ready queue
EOperationStatus appendThreadToReadyQueue(TCB_t* threadToAppend);

// Dispatcher
// Move the first ready thread with the highest priority to the executing queue
// Should only be used after the queues have been initialized
// This function should not remove the executing thread, as it is non preemptive
// The user should swapcontext after its use
EOperationStatus dispatch();

// Sets the iterator to the first available thread from the ready queue
EOperationStatus GetFirstReadyThread(PFILA2* queueReference);

// Function that will be called once a thread ends, must do the necessary logic for the cjoin function
// This function never returns, as it just swaps the context to the next thread
// It will call the scheduler to call the next thread
// It will be inside a context, which the other threads need to link to
void threadEndFunction();

#endif // !SCHEDULER_H