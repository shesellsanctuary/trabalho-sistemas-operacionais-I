#ifndef EOPERATIONSTATUS_H
#define EOPERATIONSTATUS_H

// Enum for operation status (for functions), its easier this way to keep return numbers meaningful
typedef enum EOperationStatus
{
	OpSuccess = 0, // Operation was successfull
	OpUknownError = -1, // Uknown error
	OpAllocError = -2, // Malloc could not allocate memory
	OpNullStructError = -3, // Null struct received
	OpDeleteError = -4, // Error deleting element from list
	OpInvalidPriorityError = -5, // Invalid thread priority found
	OpAppendError = -6, // Could not append to queue
	OpInvalidPrio = -7, // Invalid priority
	OpThreadDoNotExist = -8 // Searched tid not found
} EOperationStatus;

#endif // !EOPERATIONSTATUS_H