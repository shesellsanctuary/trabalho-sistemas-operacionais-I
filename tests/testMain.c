#include <stdio.h>
#include "ETestStatus.h"
#include "testMain.h"
#include "testExample.h"
#include "sourceExample.h"

ETestStatus runAllTests()
{
	// Test status variable
	ETestStatus testStatus = Success;

	// Example on how to run and check the tests (when it works)
	printf("testExampleSuccess: 5 * 6 = %d\t", testExampleSuccess(5, 6));
	if (testExampleSuccess(5, 6) != 30)
	{
		testStatus = TestError;
		printf("FAILED\tCODE = %d\n", testStatus);
	}
	else
	{
		printf("SUCCESS\n");
	}
	// Example on how to run and check the tests (when it doesn't works)
	// Purposefully making a error, calling the function that sums when I expect it to multiply
	printf("testExampleFailure: 2 * 3 = %d\t", testExampleFailure(2, 3));
	if (testExampleFailure(2, 3) != 6)
	{
		testStatus = TestError;
		printf("FAILED\tCODE = %d\n", testStatus);
	}
	else
	{
		printf("SUCCESS\n");
	}
	// Example on how to run a function from a file in the src folder
	printf("example: 1 + 2 = %d\t", example(1, 2));
	if (example(1, 2) != 3)
	{
		testStatus = TestError;
		printf("FAILED\tCODE = %d\n", testStatus);
	}
	else
	{
		printf("SUCCESS\n");
	}

	return testStatus;
}