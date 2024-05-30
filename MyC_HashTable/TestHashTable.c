#undef MyC_Test

#include <assert.h>
#include <stdio.h>

#include "HashTable.h"

static Result HashTable_InsertItem_AbleToGetBack()
{
	// Arrange
	HashTable* table;
	try (new1(HashTable, 50))
	out (table)

	Array* key;
	try (new2(Array, 4, 1))
	out (key)

	Array* key2;
	try (new2(Array, 4, 1))
	out (key2)

	int keyValue = 48;
	int value = 42;

	try (Array_Set(key, 0, &keyValue))
	end;

	// Act
	try (HashTable_Upsert(table, key, &value))
	end;

	// Assert
	try (Array_Set(key2, 0, &keyValue))
	end
	try (HashTable_Search(table, key2))
	end
	assert(value == 42);

	// Annihilate
	delete(HashTable, table);
	delete(Array, key);
	delete(Array, key2);
}
static void HashTable_InsertItemTwice_WHAT_TO_DO()
{
	// Two strategies:
	// 1. multiple same keys exist
	//		Problem: which key to update afterwards? first one?
	// 2. upsert the key if exists (update)
}

#include "TestHashTable.c.gen"