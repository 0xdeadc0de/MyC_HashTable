#include <stdio.h>

#include "TestList.h.gen"
#include "TestArray.h.gen"
#include "TestHashTable.h.gen"
#include "TestHashTableItem.h.gen"

#include "Array.h"

int main()
{
	//Array* ret __attribute__ ((cleanup(Array_Destructor))) = Array_Constructor2(NULL, 0, 0);

	//Array_Set(NULL, 0, NULL);
	
	//new2(Array, 1, 2);
puts("start");
	TestArray_RunAll();
	TestList_RunAll();
	TestHashTableItem_RunAll();
	TestHashTable_RunAll();

	puts("OK");
	
	return 0;
}