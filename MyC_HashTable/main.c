#include <stdio.h>

#include "TestMyC.c.gen"

int main()
{
	puts("Start");

	TestMyC();
		
	puts("End, OK");
	puts("");
	return 0;
}