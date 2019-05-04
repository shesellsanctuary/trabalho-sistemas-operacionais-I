#include <stdlib.h>
#include "cthread.h"
#include "EOperationStatus.h"
#include "TBool.h"
#include "utils.h"
#include "scheduler.h"

/******************************************************************************
Parâmetros:
	tid:	identificador da thread cujo término está sendo aguardado.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int cjoin(int tid)
{
	// Operation return code
	EOperationStatus returnCode = OpUknownError;

	// TID exist?
	TBool tidExist = false;

	// TID being waited for?
	TBool tidBeingWaited = false;

	// Search for the tid to see if thread exists
	if (SearchFila2(g_HighPrioReadyQueue, tid, 1) == OpSuccess) tidExist = true;
	else if (SearchFila2(g_MediumPrioReadyQueue, tid, 1) == OpSuccess) tidExist = true;
	else if (SearchFila2(g_LowPrioReadyQueue, tid, 1) == OpSuccess) tidExist = true;
	else if (SearchFila2(g_blockedQueue, tid, 1) == OpSuccess) tidExist = true;

	// Search the cjoin queue to make sure there are no other threads waiting for this tid
	if (SearchFila2(g_cjoinQueue, tid, 1) == OpSuccess) tidBeingWaited = true;

	// if tid exists and it is not being already waited for, we should block this one and the next thread with the highest priority
	if ((tidExist == true) && (!tidBeingWaited))
	{
		// Should create pair to add to cjoinqueue
		pair* newPair = (pair*)malloc(sizeof(pair));
		// p1 is the tid being waited for
		newPair->p1 = tid;
		// p2 is the tid waiting
		newPair->p2 = ((TCB_t*)g_executingThread->first->node)->tid;
		// Add to cjoinqueue and blocked queue
		if ((AppendFila2(g_cjoinQueue, newPair) == OpSuccess) 
			&& (AppendFila2(g_blockedQueue, g_executingThread->first->node) == OpSuccess))
		{
			// Remove this from execution
			if ((FirstFila2(g_executingThread) == OpSuccess) 
				&& (DeleteAtIteratorFila2(g_executingThread) == OpSuccess))
			{
				// Call dispatcher for the next thread
				dispatch();
				// Swap context
				swapcontext(&((TCB_t*)g_blockedQueue->last->node)->context, 
					&((TCB_t*)g_executingThread->last->node)->context);
				returnCode = OpSuccess;
			}
		}
	}
	// else we return error
	else
	{
		returnCode = OpThreadDoNotExist;
	}

	return returnCode;
}