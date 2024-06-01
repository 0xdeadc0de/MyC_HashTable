#undef MyC_Test

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "HashTable.h"

static Result HashTable_InsertItem_AbleToGetBack()
{
	// Arrange
	setup();
	try (ref, table, new1(HashTable, 50));
	try (ref, key, new2(Array, 4, 1));
	try (ref, key2, new2(Array, 4, 1));

	int keyValue = 48;
	int value = 42;

	run (Array_Set(key, 0, &keyValue));

	// Act
	run (HashTable_Upsert(table, key, &value));

	// Assert
	run (Array_Set(key2, 0, &keyValue));
	run (HashTable_Search(table, key2));
	assert(value == 42);

	// Annihilate
	delete(HashTable, table);
	delete(Array, key);
	delete(Array, key2);
	
	return ok;
}

static Result HashTable_InsertSameKeyTwice_UpsertsFirst()
{
	// Arrange
	setup();
	try (ref, table, new1(HashTable, 50));
	try (ref, key, new2(Array, 4, 1));
	int first = 42;
	int second = 53;

	// Act
	run (HashTable_Upsert(table, key, &first));
	try (ref, actual1, HashTable_Search(table, key));
	run (HashTable_Upsert(table, key, &second));
	try (ref, actual2, HashTable_Search(table, key));

	// Assert
	assert(actual1 == &first);
	assert(actual2 == &second);

	// Annihilate
	delete(HashTable, table);
	delete(Array, key);

	return ok;
}

static Result HashTable_DeletedItem_ShouldReturnNull()
{
	// Arrange
	setup();
	try (ref, table, new1(HashTable, 50));
	try (ref, key, new2(Array, 4, 1));

	int keyValue = 48;
	int value = 42;

	run (Array_Set(key, 0, &keyValue));

	// Act
	run (HashTable_Upsert(table, key, &value));
	run (HashTable_Delete(table, key));

	// Assert
	try (ref, r, HashTable_Search(table, key));
	assert(NULL == r);

	// Annihilate
	delete(HashTable, table);
	delete(Array, key);

	return ok;
}

static Result HashTable_InsertLargeAmountOfDataThenDelete_HopeItDoesNotCrash()
{
	// Benchmark start
	struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

	// Arrange
	setup();
	try (ref, table, new(HashTable));
	int value = 5;
	
	const size_t N = 10000000;
	const size_t keySize = ceil(log10(N))+2;

	try (ref, r, new2(Array, sizeof(void*), N));
	Array* keys = r;

	// Act & Assert

	for (size_t i = 0; i < N; i++)
	{
		// Create a new key
		try (ref, k, new2(Array, sizeof(char), keySize));
		Array* key = k;
		sprintf(key->Items, "%llu", i);

		// Store the reference in keys
		run (Array_Set(keys, i, &key));

		// Insert it in hash table
		run (HashTable_Upsert(table, key, &value+i));
		
		// Assert: able to get value back
		try (ref, item, HashTable_Search(table, key));
		assert(item == &value+i);
	}

	for (size_t i = 0; i < N; i++)
	{
		// Get the key
		try (ref, k, Array_At(keys, i));
		Array* key = *((void**)k);

		// Assert: still able to find the key, and it contains correct value.
		try (ref, refValue, HashTable_Search(table, key));
		assert(refValue == &value+i);

		// Delete the item
		run (HashTable_Delete(table, key));

		// Assert: shouldn't be able to find the key.
		try (ref, expectNull, HashTable_Search(table, key));
		assert(NULL == expectNull);

		// Destroy the key.
		delete(Array, key);
	}

	// Annihilate
	delete(Array, keys);
	delete(HashTable, table);

	

    // Benchmark end
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

    // Print the results
    printf("Execution time: %f seconds\n, N: %llu", elapsed, N);

	return ok;
}


#include "TestHashTable.c.gen"