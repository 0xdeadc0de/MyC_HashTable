#include <stdio.h>

#include "TestList.h.gen"
#include "TestArray.h.gen"
#include "TestHashTable.h.gen"
#include "TestHashTableItem.h.gen"

#include "Array.h"

int main()
{
	//Array* ret __attribute__ ((cleanup(Array_Destructor))) = Array_Constructor2(NULL, 0, 0);

	TestArray_RunAll();
	TestList_RunAll();
	TestHashTableItem_RunAll();
	TestHashTable_RunAll();

	printf("OK\n");
	
	return 0;
}