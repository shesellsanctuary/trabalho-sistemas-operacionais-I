#ifndef UTILS_H
#define UTILS_H

#include "support.h"
#include "EOperationStatus.h"

// Searches queue for the thread with the TID == content
// At the end, the it of the queue will be positioned on the found element, else it will be null
EOperationStatus SearchThreadFila2(PFILA2 queue, int content);

// Gets the size of a queue
int GetSizeFila2(PFILA2 queue);

#endif // !UTILS_H