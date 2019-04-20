/*
	csignal from cthread.h implementation.
*/

#include <stdlib.h>
#include <errno.h>
#include "cthread.h"
#include "EOperationStatus.h"
#include "support.h"


int
csignal(csem_t *sem)
{
	// Operation status to be returned
	EOperationStatus returnCode = OpUknownError;

	// If allocation was successfull
	if (sem != NULL)
	{
	}
	else
	{
		// Null struct
		returnCode = OpNullStructError;
	}

	return returnCode;
}