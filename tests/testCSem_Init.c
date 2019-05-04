#include <stdlib.h>
#include <errno.h>
#include "cthread.h"
#include "ETestStatus.h"
#include "EOperationStatus.h"
#include "testCSem_Init.h"

ETestStatus csem_init_test()
{
	// Test status
	ETestStatus testStatus = TestSuccess;

	//  Pointer to semaphore
	// Allocates memory
	csem_t* pSem = (csem_t*)malloc(sizeof(csem_t));

	// Number of available slots for semaphore
	// Constant, do not change during execution of the test
	int c_count = 10;

	// Atempts to initialize
	if (csem_init(pSem, c_count) != OpSuccess)
	{
		testStatus = TestError;
	}
	else
	{
		// Now we test the pSem to see if it is different than NULL
		if (errno == ENOMEM)
		{
			testStatus = TestError;
		}
		// Test the attributes
		else
		{
			// Test the count attribute
			if (pSem->count != c_count)
			{
				testStatus = TestError;
			}
			// Test the queue attribute
			if (pSem->fila == NULL) // Should at least be 0x4 -> 4 bytes after int
			{
				testStatus = TestError;
			}
		}
	}

	// !!!  IMPORTANT !!!
	// Needs to free the memory used always
	// Free the queue
	free(pSem->fila);
	// Free the semaphore
	free(pSem);

	return testStatus;
}