#include <stdio.h>

#include "TestList.h.gen"
#include "TestArray.h.gen"
#include "TestHashTable.h.gen"
#include "TestHashTableItem.h.gen"

int main()
{
	$TestArray.RunAll();
	$TestList.RunAll();
	$TestHashTableItem.RunAll();
	$TestHashTable.RunAll();
	return 0;
}