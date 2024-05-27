#undef MyC_Test

#include <assert.h>

#include "Array.h"
#include "HashTableItem.h"

static void HashTableItem_Constructor_CreatesExactCopyOfKeys()
{
	// Assert
	Array* key1 = new2(Array, sizeof(char), 10);

	// Act
	HashTableItem* item = new2(HashTableItem, key1, &key1);

	// Assert
	assert($Array.Equals(key1, item->Key));
}

#include "TestHashTableItem.c.gen"