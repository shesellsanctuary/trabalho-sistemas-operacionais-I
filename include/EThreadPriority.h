#ifndef ETHREADPRIORITY_H
#define ETHREADPRIORITY_H

// Enum for test status, its easier this way to keep return numbers meaningful
typedef enum EThreadPriority
{
	ThreadHighPriority = 0, // Thread has high priority
	ThreadMediumPriority = 1, // Thread has medium priority
	ThreadLowPriority = 2 // Thread has low priority
} EThreadPriority;

#endif // !ETHREADPRIORITY_H