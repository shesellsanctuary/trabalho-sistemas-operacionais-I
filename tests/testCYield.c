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

int x1 = 0;

void dummyF1()
{
	cyield();
	x1++;
}

ETestStatus cyield_test()
{
    // Initializes library
	initialize();

    // Test status
    ETestStatus testStatus = TestSuccess;

    // Create executing thread to free the cpu willingly
	ccreate((void*)dummyF1, NULL, ThreadHighPriority);
	// Create executing thread to free the cpu willingly
	ccreate((void*)dummyF1, NULL, ThreadHighPriority);

	// Main thread must yield first
    if (cyield() != OpSuccess)
    {
        testStatus = TestError;
        perror("Fail yielding.\n");
    }
    else
    {
        // Both threads should have finished
		// It means both were in the blocked state and in the correct queue, as both have finished before the main thread
        if (GetSizeFila2((PFILA2)g_HighPrioReadyQueue) != 0)
        {
            perror("Threads have not finished.\n");
            testStatus = TestError;
        }
		// Verify if both threads finished
		if (x1 != 2)
		{
			perror("Have not swapped context.\n");
			testStatus = TestError;
		}
    }

    // !!!  IMPORTANT !!!
    // Needs to free the memory used always

    return testStatus;
}