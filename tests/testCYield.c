#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
#include "scheduler.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "EThreadPriority.h"
#include "utils.h"
#include <stdio.h>

ETestStatus cyield_test()
{
    // Initializes library
	initialize();

    // Test status
    ETestStatus testStatus = TestSuccess;

    // Create executing thread to free the cpu willingly
    
    TCB_t *x = (TCB_t *)malloc(sizeof(TCB_t));
    ucontext_t *context = (ucontext_t *)malloc(sizeof(ucontext_t));
    getcontext(context);
    x->context = *context;
    x->prio = ThreadHighPriority;
    x->state = PROCST_APTO;
    x->tid = g_numOfThreads++;
	
	appendThreadToReadyQueue(x);
    
	// Main thread must yield first
    if ((cyield() != OpSuccess) || (cyield() != OpSuccess))
    {
        testStatus = TestError;
        perror("Fail yielding.\n");
    }
    else
    {
        // x should have apto state
        if (x->state != PROCST_APTO)
        {
            testStatus = TestError;
            perror("Wrong new state.\n");
        }
        // Thread should be in its respective ready queue
        if (SearchThreadFila2(g_HighPrioReadyQueue, x->tid) != OpSuccess)
        {
            perror("Thread not found in ready queue.\n");
            testStatus = TestError;
        }
    }

    // !!!  IMPORTANT !!!
    // Needs to free the memory used always
    // free(x);
    // free(context);

    return testStatus;
}