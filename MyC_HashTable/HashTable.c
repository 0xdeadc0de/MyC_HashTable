#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "HashTable.h"
#include "HashTableItem.h"

#include "HashTable.head.c.gen"

TypedefResult(size_t);

// Allocate a memory location to use as a sentinel value for items marked as deleted (similar to NULL pointer)
static void* DELETED = &DELETED;

static size_t findNextPrime(size_t x)
{
	// Just return 7, we don't want to deal with small sizes
	if (x < 7)
	{
		return 7;
	}

	if (0 == x % 2)
	{
		// If its even number, increase it, evens can't be primes
		x++;
	}
	else
	{
		// We want to find next prime, if number is odd, increase by 2
		x += 2;
	}

	// Lowest x is 9, sqrt(9) = 3, 3 <= 3 will run once at least
	for (size_t i = 3; i <= floor(sqrt((double)x)); i += 2)
	{
		if (0 == (x % i))
		{
			i = 1;
			x += 2;
		}
	}

	return x;
}
static int hash(const char* key, size_t n, size_t prime, size_t size)
{
	int hash = 0;
	for (size_t i = 0; i < n; i++)
	{
		hash += ((size_t)pow(prime, (double)i) % size) * (key[i] % size);
		hash %= size;
	}

	return hash;
}
// Open addressing, we will use double hashing
static size_t doubleHash(const Array* key, size_t size, size_t attempt)
{
	// We're hashing ASCII strings, which has an alphabet size of 128 
	// We should choose a prime larger than that 151, 163 should do
	const size_t firstHash  = hash(key->Items, Array_Length(key), 151, size);
	const size_t secondHash = hash(key->Items, Array_Length(key), 163, size-1) + 1;

	size_t finalHash = firstHash + attempt * secondHash;
	finalHash %= size;

	return finalHash;
}
static Result(size_t) search(HashTable* self, const Array* key, bool findInsertLocation)
{
	size_t attempt = 0;
	// Search until we find the key matching or we find an empty slot
	for (;;)
	{
		size_t index = doubleHash(key, getSize(self), attempt);
		attempt++;

		setup(size_t);
		try (ref, item, List_At(getList(self), index));

		if (NULL == item)
		{
			return OK(index);
		}

		if (DELETED == item)
		{
			if (findInsertLocation)
			{
				return OK(index);
			}

			// We assume deleted items as legitimate items in the table to avoid breaking the chain of double hashing
			// Therefore we continue when we want to locate an item or delete an item
			continue;
		}

		// There is no point to compare keys if we want to find an empty slot
		if (findInsertLocation)
		{
			continue;
		}
		
		HashTableItem* hashTableItem = item;
		if (Array_Equals(key, hashTableItem->Key))
		{
			return OK(index);
		}
	}
}
static Result(ref) resize(HashTable* self, size_t newSize)
{
	if (newSize < 50)
	{
		return (Result(ref)) {OK, self};
	}

	// Allocate a new table, and copy item pointers
	setup(ref);
	try (ref, r, new1(HashTable, newSize));
	HashTable* newTable = r;

	for (size_t i = 0; i < getSize(self); i++)
	{
		try (ref, r, List_At(getList(self), i));
		HashTableItem* item = r;
		
		// If an item is deleted or null, skip it
		if (NULL == item || DELETED == item)
		{
			continue;
		}

		run (HashTable_Upsert(newTable, item->Key, item->Value));
	}

	// Swap tables so we can free memory on new table pointer
	size_t tempSize = getSize(self); 
	setSize(self, getSize(newTable));
	setSize(newTable, tempSize);

	List* tempList = getList(self);
	setList(self, getList(newTable));
	setList(newTable, tempList);

	// The strategy is we modify the table passed as argument
	// And free the new table with swapped memory locations
	delete(HashTable, newTable);

	return (Result(ref)) {OK, self};
}

// Constructs a HashTable and returns the pointer
Result(ref) HashTable_Constructor1(HashTable* self, size_t size)
{
	size = findNextPrime(size);

	setup(ref);
	try (ref, r, new1(List, size));
	List* list = r;

	*self = (HashTable)
	{
		.Count = 0,
		
		._size = size,
		._list = list
	};
	
	for (size_t i = 0; i < size; i++)
	{
		run (List_PushBack(getList(self), NULL));
	}

	return (Result(ref)) {OK, self};
}
// Frees the resources held, and returns reference to self
HashTable* HashTable_Destructor(HashTable* self)
{
	delete(List, getList(self));
	return self;
}
// Inserts key, value returns self. If key exists, updates the value
Result(ref) HashTable_Upsert(HashTable* self, const Array* key, void* value)
{
	setup(ref);

	if (70 < self->Count * 100 / getSize(self))
	{
		run (resize(self, getSize(self) * 2));
 	}

	// Search an empty slot location to insert the new item
	try (size_t, index, search(self, key, true));

	try (ref, newPair, new2(HashTableItem, key, value));

	// Set item
	run (List_Set(getList(self), index, newPair));

	// Increase the count of items in the table
	self->Count++;

	return (Result(ref)) {OK, self};
}
// Deletes the key from the hash table and returns self
Result(ref) HashTable_Delete(HashTable* self, const Array* key)
{
	setup(ref);
	try (size_t, index, search(self, key, false));
	
	try (ref, item, List_At(getList(self), index));
	
	// If search returned an empty slot location, we do nothing, this item doesn't exist in table already
	if (NULL == item)
	{
		return (Result(ref)) {OK, self};
	}

	run (List_Set(getList(self), index, DELETED));
	delete(HashTableItem, item);

	self->Count--;

	if (10 > self->Count * 100 / getSize(self))
	{
		return resize(self, getSize(self) / 2);
	}

	return (Result(ref)) {OK, self};
}
// Searches and retrieves the value from given key if exists
Result(ref) HashTable_Search(HashTable* self, const Array* key)
{
	// Search for the item with the given key
	setup(ref);
	try (size_t, index, search(self, key, false));

	try (ref, r, List_At(getList(self), index));
	HashTableItem* item = r;

	// Return the value of the item found
	return (Result(ref)) {OK, item->Value};
}