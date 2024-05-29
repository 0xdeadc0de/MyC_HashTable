#undef MyC_Test

#include <assert.h>
#include <stdio.h>

#include "HashTable.h"

static void HashTable_InsertItem_AbleToGetBack()
{
	// Arrange
	HashTable* table = new1(HashTable, 50);
	Array* key = new2(Array, 4, 1);
	Array* key2 = new2(Array, 4, 1);

	int keyValue = 48;
	int value = 42;

	$Array.Set(key, 0, &keyValue);

	// Act
	$HashTable.Upsert(table, key, &value);

	// Assert
	$Array.Set(key2, 0, &keyValue);
	$HashTable.Search(table, key2);
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
	
	// Arrange
	HashTable* table = new1(HashTable, 50);
	Array* key = new2(Array, 4, 1);
	int keyValue = 48;
	int value = 42;
	$Array.Set(key, 0, &keyValue);

	// Act
	$HashTable.Upsert(table, key, &value);

	// Assert
	Array* key2 = new2(Array, 4, 1);
	$Array.Set(key2, 0, &keyValue);
	$HashTable.Search(table, key2);
	assert(value == 42);
}

#include "TestHashTable.c.gen"