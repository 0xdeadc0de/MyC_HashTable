#undef MyC_Test

#include <assert.h>

#include "Array.h"
#include "HashTableItem.h"

static Result HashTableItem_Constructor_CreatesExactCopyOfKeys()
{
	// Assert
	Array* key1;
	try (new2(Array, sizeof(char), 10))
	set (key1)

	// Act
	HashTableItem* item;
	try (new2(HashTableItem, key1, &key1))
	set (item);

	// Assert
	assert(Array_Equals(key1, item->Key));
}

#include "TestHashTableItem.c.gen"