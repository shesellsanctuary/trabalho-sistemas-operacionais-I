#include <stdlib.h>
#include <ucontext.h>
#include "cthread.h"
#include "cdata.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "testCSignal.h"
#include "EThreadPriority.h"

#include <stdio.h>

#define MAX_SIZE 100

ETestStatus cidentify_test()
{
	// Test status
	ETestStatus testStatus = TestSuccess;

	char names[MAX_SIZE];
	//char names = "Arthur Zachow - 00277951\nEduardo Guerra - 00260267\nEmily Calvet - 00242277";
	
	int i = 0;
	for (; i < MAX_SIZE; i++) {
      names[i] = 'T';
    }

    if (cidentify(names, MAX_SIZE) != 0)
	{
		testStatus = TestError;
	}

	return testStatus;
}