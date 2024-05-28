#undef MyC_Test

#include <assert.h>

#include "HashTable.h"

static void HashTable_InsertItem_AbleToGetBack()
{
	// Arrange
	HashTable* table = new1(HashTable, 50);
	Array* key = new2(Array, 4, 1);
	int keyValue = 48;
	int value = 42;
	$Array.Set(key, 0, &keyValue);

	// Act
	$HashTable.Insert(table, key, &value);

	// Assert
	Array* key2 = new2(Array, 4, 1);
	$Array.Set(key2, 0, &keyValue);
	$HashTable.Search(table, key2);
	assert(value == 42);
}

#include "TestHashTable.c.gen"