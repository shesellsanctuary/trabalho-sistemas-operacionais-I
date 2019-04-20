#ifndef ETESTSTATUS_H
#define ETESTSTATUS_H

// Enum for test status, its easier this way to keep return numbers meaningful
typedef enum ETestStatus
{
	TestSuccess = 0, // Test was successfull
	TestError = -1 // Test failure
} ETestStatus;

#endif // !ETESTSTATUS_H