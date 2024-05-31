#undef MyC_Test

#include <assert.h>
#include <stdio.h>

#include "HashTable.h"

static Result(ref) HashTable_InsertItem_AbleToGetBack()
{
	// Arrange
	HashTable* table;
	try_old (new1(HashTable, 50))
	out_old (table)

	Array* key;
	try_old (new2(Array, 4, 1))
	out_old (key)

	Array* key2;
	try_old (new2(Array, 4, 1))
	out_old (key2)

	int keyValue = 48;
	int value = 42;

	ret (ref);
	run (Array_Set(key, 0, &keyValue));;

	// Act
	run (HashTable_Upsert(table, key, &value));;

	// Assert
	run (Array_Set(key2, 0, &keyValue));
	run (HashTable_Search(table, key2));
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