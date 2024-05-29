#include <stdio.h>

#include "TestList.h.gen"
#include "TestArray.h.gen"
#include "TestHashTable.h.gen"
#include "TestHashTableItem.h.gen"

#include "Array.h"

int main()
{
	//Array* ret __attribute__ ((cleanup(Array_Destructor))) = $Array.Constructor2(NULL, 0, 0);

	$TestArray.RunAll();
	$TestList.RunAll();
	$TestHashTableItem.RunAll();
	$TestHashTable.RunAll();

	printf("OK\n");
	
	return 0;
}