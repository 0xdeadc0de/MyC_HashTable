#undef MyC_Test

#include "Array.h"

static void HashTableItem_Constructor_CreatesExactCopyOfKeys()
{
	// Assert
	Array* key1 = new2(Array, sizeof(char), 10);
	
}

#include "TestHashTableItem.c.gen"