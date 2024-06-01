#undef MyC_Test

#include <assert.h>

#include "Array.h"
#include "HashTableItem.h"

static Result HashTableItem_Constructor_CreatesExactCopyOfKeys()
{
	// Assert
	setup();
	try (ref, key1, new2(Array, sizeof(char), 10));
	Array* a1 = key1;

	// Act
	try (ref, item, new2(HashTableItem, key1, &key1));
	HashTableItem* h = item;

	// Assert
	assert(Array_Equals(a1, h->Key));
	
	return ok;
}

#include "TestHashTableItem.c.gen"