#include "tools.h"
#include "config.h"
#include<stdio.h>

int WideToServoDuty(int wide_duty)
{
	return 25 + ((wide_duty - 100) * 100) / 800;
}

void DebugInfo(char* message)
{
	if(DEBUG_MODE)
		printf("%s", message);
}