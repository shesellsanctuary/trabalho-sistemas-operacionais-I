#include <stdlib.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
#include "EOperationStatus.h"

extern int g_numOfThreads;


static int tid = 0;


int get_tid()
{
    tid++;
    return tid;
}


int ccreate (void* (*start)(void*), void *arg, int prio)
{

    TCB_t* new_thread = malloc(sizeof(TCB_t));

    new_thread->tid = get_tid();
    new_thread->state = PROCST_APTO;
    new_thread->prio = prio;

    //definir o contexto
    
    getcontext(&new_thread->context);

    new_thread->context.uc_stack.ss_size = STACK_SIZE;
    new_thread->context.uc_stack.ss_size = malloc(sizeof(STACK_SIZE));

    makecontext(&new_thread->context, (void (*)(void)) start, NUM_ARG, arg);

    //adicionar nova thread a fila de threads

    switch(prio)
    {
        case 0:
            AppendFila2(g_HighPrioReadyQueue, new_thread);
            break;
        case 1:
            AppendFila2(g_MediumPrioReadyQueue, new_thread);
            break;
        case 2:
            AppendFila2(g_LowPrioReadyQueue, new_thread);
            break;
        default:
            return -1;
            break;   
    }
    

    return new_thread->tid;
}

