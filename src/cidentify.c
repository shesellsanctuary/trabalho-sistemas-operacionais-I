/*
	cidentify from cthread.h implementation.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"
#include "cthread.h"
#include "cdata.h"
#include "EOperationStatus.h"

int 
cidentify (char *name, int size)
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
        strncpy (name, "Arthur Zachow - 00277951\nEduardo Guerra - 00260267\nEmily Calvet - 00242277", size);
		return 0;
    }
    return returnCode;
}