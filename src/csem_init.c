/*
	csem_init from cthread.h implementation.
*/

#include <stdlib.h>
#include <errno.h>
#include "scheduler.h"
#include "cthread.h"
#include "EOperationStatus.h"
#include "support.h"

/******************************************************************************
Parâmetros:
	sem:	ponteiro para uma variável do tipo csem_t. Aponta para uma estrutura de dados que representa a variável semáforo.
	count: valor a ser usado na inicialização do semáforo. Representa a quantidade de recursos controlados pelo semáforo.
Retorno:
	Quando executada corretamente: retorna 0 (zero)
	Caso contrário, retorna um valor negativo.
******************************************************************************/
int
csem_init(csem_t *sem, int count)
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

		// If semaphore is a valid struct
		if (sem != NULL)
		{
			// sets semaphore counter
			sem->count = count;
			// Allocates queue
			sem->fila = (PFILA2)malloc(sizeof(FILA2));
			// sets semaphore queue
			if ((errno != ENOMEM) && (CreateFila2(sem->fila) == 0))
			{
				// operation was successfull
				returnCode = OpSuccess;
			}
			else
			{
				// Could not allocate queue
				returnCode = OpAllocError;

				// If the operation failed and we allocated memory, we need to free it
				free(sem);
			}
		}
		else
		{
			// Null struct
			returnCode = OpNullStructError;
		}
	}

	return returnCode;
}