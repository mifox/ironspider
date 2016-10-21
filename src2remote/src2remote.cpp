#include "src2remote.h"

/*
	To test the library, include "src2remote.h" from an application project
	and call src2remoteTest().
	
	Do not forget to add the library to Project Dependencies in Visual Studio.
*/

static int s_Test = 0;

extern "C" int src2remoteTest();

int src2remoteTest()
{
	return ++s_Test;
}