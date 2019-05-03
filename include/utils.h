#ifndef UTILS_H
#define UTILS_H

#include "support.h"
#include "EOperationStatus.h"

// Defines a pair structure
typedef struct s_pair {
	int		p1; 	// first member
	int		p2;		// second member
} pair;

// Searches queue for the thread with the TID == content
// At the end, the it of the queue will be positioned on the found element, else it will be null
EOperationStatus SearchThreadFila2(PFILA2 queue, int content);

// Searches queue for the pair where p1 == content
// At the end, the it of the queue will be positioned on the found element, else it will be null
EOperationStatus SearchFila2(PFILA2 queue, int content);

// Gets the size of a queue, returns 0 for NULL
int GetSizeFila2(PFILA2 queue);

#endif // !UTILS_H