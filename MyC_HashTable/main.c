#include <stdio.h>

#include "TestList.h.gen"
#include "TestArray.h.gen"
#include "TestHashTableItem.h.gen"

int main()
{
	$TestArray.RunAll();
	$TestList.RunAll();
	$TestHashTableItem.RunAll();
	return 0;
}