// We use this to avoid multiple main definitions
#ifdef 
// Here we call the runAllTests function defined in testMain
#include <stdio.h>
#include "sourceExample.h"

int main(int argc, char* argv[])
{
	// Just call the run all tests and if it returns != there's error
	return runAllTests();
}
#else
#include <stdio.h>
#include "sourceExample.h"

int main(int argc, char* argv[])
{
	printf("1 + 2 = %d\n", example(1, 2));

	return 0;
}

#endif