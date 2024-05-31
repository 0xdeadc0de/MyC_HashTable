#undef MyC_Test

#include <assert.h>

#include "Array.h"
#include "HashTableItem.h"

static Result HashTableItem_Constructor_CreatesExactCopyOfKeys()
{
	// Assert
	Array* key1;
	try_old (new2(Array, sizeof(char), 10))
	out_old (key1)

	// Act
	HashTableItem* item;
	try_old (new2(HashTableItem, key1, &key1))
	out_old (item);

	// Assert
	assert(Array_Equals(key1, item->Key));
}

#include "TestHashTableItem.c.gen"