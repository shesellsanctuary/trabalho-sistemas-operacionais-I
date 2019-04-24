#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "support.h"
#include "cdata.h"
#include "EOperationStatus.h"

#define MAIN_TID 0
// Each of the ready queues sub queue
#define g_HighPrioReadyQueue g_readyQueues->first->node
#define g_MediumPrioReadyQueue g_readyQueues->first->next->node
#define g_LowPrioReadyQueue g_readyQueues->first->next->next->node

// Queues with the ready threads, one for each priority
PFILA2 g_readyQueues;

// Blocked threads
PFILA2 g_blockedQueue;

// Executing thread
PFILA2 g_executingThread;

// Number of total threads created since the beginning 
int g_numOfThreads;

// Initialize the library and its queues and necessary stuff
EOperationStatus initialize();

// Appends to the right ready queue
EOperationStatus appendThreadToReadyQueue(TCB_t* threadToAppend);

#endif // !SCHEDULER_H