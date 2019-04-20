#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "support.h"
#include "EOperationStatus.h"
#define MAINTID 0

// Queue with the ready threads
PFILA2 g_readyQueue;

// Blocked threads
PFILA2 g_blockedQueue;

// Executing thread
PFILA2 g_executingThread;

// Number of total threads created since the beginning 
int g_numOfThreads = 0;

// Initialize the library and its queues and necessary stuff
EOperationStatus initialize();

#endif // !SCHEDULER_H