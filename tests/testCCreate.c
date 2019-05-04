#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include "cthread.h"
#include "scheduler.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "EThreadPriority.h"
#include "utils.h"

int dummyF2()
{
	return 2;
}

ETestStatus ccreate_test()
{
    // Initializes library
	initialize();

    // Test status
    ETestStatus testStatus = TestSuccess;

	int tid  = ccreate((void*)dummyF2, NULL, ThreadHighPriority);

	// Thread should be on its respective ready queue
    if (SearchThreadFila2(g_HighPrioReadyQueue, tid) != OpSuccess)
    {
        testStatus = TestError;
        perror("Thread not in ready queue.\n");
    }
    else
    {
        TCB_t* thread = ((PFILA2)g_HighPrioReadyQueue)->first->node;
		// Thread should have right prio and state
        if ((thread->state != PROCST_APTO) || (thread->prio != ThreadHighPriority))
        {
            perror("Threads state or prio is wrong.\n");
            testStatus = TestError;
        }
		// Verify if both threads finished
		if (thread->context.uc_link != &endOfThreadContext)
		{
			perror("Thread does not have uclink in context.\n");
			testStatus = TestError;
		}
        // !!!  IMPORTANT !!!
        // Needs to free the memory used always
        free(thread);
    }

    return testStatus;
}