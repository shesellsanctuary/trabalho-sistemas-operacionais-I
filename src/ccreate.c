#include <stdlib.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
#include "EOperationStatus.h"

extern int g_numOfThreads;

int ccreate(void *(*start)(void *), void *arg, int prio)
{
    // Operation status to be returned
    EOperationStatus returnCode = OpUknownError;
    // Initialization return code
    EOperationStatus initializationCode = OpSuccess;
    // Check if we need to initialize the library
    if (g_numOfThreads == 0)
    {
        initializationCode = initialize();
    }

    if (initializationCode == OpSuccess)
    {
        TCB_t *new_thread = malloc(sizeof(TCB_t));

        new_thread->tid = ++g_numOfThreads;
        new_thread->state = PROCST_APTO;
        new_thread->prio = prio;

        // Define context
        getcontext(&new_thread->context);

        new_thread->context.uc_stack.ss_size = STACK_SIZE;
        new_thread->context.uc_stack.ss_sp = malloc(STACK_SIZE);
		new_thread->context.uc_link = &endOfThreadContext;

        makecontext(&new_thread->context, (void (*)(void))start, NUM_ARG, arg);

        // Add new thread to respective priority queue
        if (appendThreadToReadyQueue(new_thread) == OpSuccess)
        {
            returnCode = new_thread->tid;
        }
    }
    return returnCode;
}