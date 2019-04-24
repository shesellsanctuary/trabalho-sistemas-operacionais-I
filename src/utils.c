#include <stdlib.h>
#include "cdata.h"
#include "utils.h"
#include "TBool.h"

// Searches queue for the thread with the TID == content
// At the end, the it of the queue will be positioned on the found element, else it will be null
EOperationStatus SearchThreadFila2(PFILA2 queue, int content)
{
	// Operation status
	EOperationStatus retStatus = OpUknownError;

	// If allocation was successfull
	if (queue != NULL)
	{
		// Initializing iterator
		if (FirstFila2(queue) == 0)
		{
			// Found flag
			TBool found = false;
			// Iterates through the queue
			while ((queue->it != queue->last->next) && (found != true))
			{
				// If the TID of the thread is equal to the content
				if ((((TCB_t*)queue->it->node)->tid == content))
				{
					found = true;
				}
				// If not found
				if (!found)
				{
					NextFila2(queue);
				}
			}
			// If the element was not found, then we set to null it
			if (!found)
			{
				queue->it = NULL;
			}
			// Operation success if it could search and try to find
			retStatus = OpSuccess;
		}
	}
	else
	{
		// Null struct received
		retStatus = OpNullStructError;
	}

	return retStatus;
}

int GetSizeFila2(PFILA2 queue)
{
	// Size of the queue
	int size = 0;

	// If allocation was successfull
	if (queue != NULL)
	{
		// Initializing iterator
		if (FirstFila2(queue) == 0)
		{
			// Iterates through the queue
			for (; queue->it != queue->last->next; NextFila2(queue))
			{
				// Adds to the size
				size++;
			}
		}
	}

	return size;
}