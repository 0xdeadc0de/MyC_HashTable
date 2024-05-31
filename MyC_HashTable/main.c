#include <stdio.h>

#include "TestList.h.gen"
#include "TestArray.h.gen"
#include "TestHashTable.h.gen"
#include "TestHashTableItem.h.gen"

#include "Array.h"

#include "List.h"

	

int main()
{
	int code;
	puts("Start");

	Result(ref) (*runAlls[])(void) = {
		TestArray_RunAll,
		TestList_RunAll,
		TestHashTableItem_RunAll,
		TestHashTable_RunAll
	};
	for (size_t i = 0; i < 4; i++) 
	{
		if (code = runAlls[i]().code) 
		{
			printf("failed with Result.code = %d", code);
			return code; 
		}
	}
		
	puts("OK");
	return 0;
}