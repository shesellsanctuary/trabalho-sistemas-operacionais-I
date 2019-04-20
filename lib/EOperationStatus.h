#ifndef EOPERATIONSTATUS_H
#define EOPERATIONSTATUS_H

// Enum for operation status (for functions), its easier this way to keep return numbers meaningful
typedef enum EOperationStatus
{
	OpSuccess = 0, // Operation was successfull
	OpUknownError = -1, // Uknown error
	OpAllocError = -2, // Malloc could not allocate memory
	OpNullStructError = -3 // Null struct received
} EOperationStatus;

#endif // !EOPERATIONSTATUS_H