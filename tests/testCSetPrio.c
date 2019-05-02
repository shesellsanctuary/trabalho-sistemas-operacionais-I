#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
#include "scheduler.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "EThreadPriority.h"
#include <stdio.h>

#define MAX_SIZE 100

ETestStatus csetprio_test()
{
	// Test status
	ETestStatus testStatus = TestSuccess;

	// Create thread with high priority
    TCB_t* x = (TCB_t*)malloc(sizeof(TCB_t));
	ucontext_t* context = (ucontext_t*)malloc(sizeof(ucontext_t));
	getcontext(context);
	x->context = *context;
	x->prio = ThreadHighPriority;
	x->state = PROCST_EXEC;
	x->tid = 1;

	// Insert into executing 
    if (AppendFila2(g_executingThread, x) != 0)
	{
		//nothing
	}
	// Change thread priority to low
    if (csetprio(0, ThreadLowPriority) != 0)
	{
		testStatus = TestError;
	}
    else{
		// Thread priority should be low
        if (x->prio != ThreadLowPriority)
        {
            testStatus = TestError;
        }
    }
    free(x);
    free(context);
	return testStatus;
}