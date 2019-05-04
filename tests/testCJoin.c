#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include "cthread.h"
#include "scheduler.h"
#include "utils.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "testCJoin.h"
#include "EThreadPriority.h"

int x = 0;

void dummyF()
{
	x++;
}

ETestStatus cjoin_test()
{
	// Initializes library
	initialize();

	// Test status
	ETestStatus testStatus = TestSuccess;

	ccreate((void*)dummyF, NULL, ThreadHighPriority);

	if (cjoin(((TCB_t*)((PFILA2)g_HighPrioReadyQueue)->first->node)->tid) != OpSuccess)
	{
		testStatus = TestError;
	}
	else
	{
		// Should have called the thread to execute dummyF and returned
		if (x != 1)
		{
			testStatus = TestError;
		}
	}

	return testStatus;
}