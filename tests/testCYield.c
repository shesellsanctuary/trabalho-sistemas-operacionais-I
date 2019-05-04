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
    // Get main thread
    TCB_t* thread = (TCB_t*)g_executingThread->first->node;
    printf("main? %d\n state: %d\n", thread->tid, thread->state);

    // Test status
    ETestStatus testStatus = TestSuccess;

    // Create executing thread to free the cpu willingly
    
    TCB_t *x = (TCB_t *)malloc(sizeof(TCB_t));
    ucontext_t *context = (ucontext_t *)malloc(sizeof(ucontext_t));
    getcontext(context);
    x->context = *context;
    x->prio = ThreadHighPriority;
    x->state = PROCST_APTO;
    x->tid = 1;

    AppendFila2(g_HighPrioReadyQueue, x);

    // if (AppendFila2(g_executingThread, x) != 0)
    // {
    //     //nothing
    // }
    if (cyield() != OpSuccess)
    {
        testStatus = TestError;
        printf("fail yielding\n");
    }
    else
    {
        printf("exec thread: %d\n", ((TCB_t*)g_executingThread->first->node)->tid);
        // First thread should have "apto" state
        if (((TCB_t*)g_executingThread->first->node)->state != PROCST_APTO)
        {
            testStatus = TestError;
            printf("wrong new state: %d \n", ((TCB_t*)g_executingThread->first->node)->state);
        }
        // Thread should be in its respective ready queue
        if (SearchThreadFila2(g_LowPrioReadyQueue, thread->tid) != 0)
        {
            printf("thread not found in ready queue\n");
            testStatus = TestError;
        }
    }

    // !!!  IMPORTANT !!!
    // Needs to free the memory used always
    // free(x);
    // free(context);

    return testStatus;
}