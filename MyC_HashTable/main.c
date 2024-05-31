#include <stdio.h>

#include "TestList.h.gen"
#include "TestArray.h.gen"
#include "TestHashTable.h.gen"
#include "TestHashTableItem.h.gen"

#include "Array.h"

#include "List.h"

int main()
{
	puts("Start");

	TestArray_RunAll();
	TestList_RunAll();
	TestHashTableItem_RunAll();
	TestHashTable_RunAll();

	puts("OK");
	
	return 0;
}