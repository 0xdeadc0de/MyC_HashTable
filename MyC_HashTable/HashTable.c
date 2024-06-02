#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

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
static Result(size_t) search(HashTable* self, const Array* key, bool returnDeleted)
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
			if (returnDeleted)
			{
				return OK(index);
			}

			// We assume deleted items as legitimate items in the table to avoid breaking the chain of double hashing
			// Therefore we continue when we want to locate an item or delete an item
			continue;
		}

		// If keys are equal, for insertion, we return to upsert.
		HashTableItem* hashTableItem = item;
		if (Array_Equals(key, hashTableItem->Key))
		{
			return OK(index);
		}
	}
}
inline static Result set(HashTable* self, size_t index, HashTableItem* value)
{
	setup();
	run (List_Set(getList(self), index, value));
	return ok;
}
static Result resize(HashTable* self, size_t newSize)
{
	setup();
	if (newSize < 50)
	{
		return ok;
	}

	// Allocate a new table, and copy item pointers
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
		
		// Insert item into new table
		try (size_t, index, search(newTable, item->Key, true));
		run (set(newTable, index, item));

		// Set list item to null so destructor does not free HashTableItem
		// We're going to keep using same HashTableItems
		run (List_Set(getList(self), i, NULL));
	}

	// Swap tables so we can free memory with using newTable
#warning What's purpose of temp size swapping? Try remove and run tests when build is clean
	size_t tempSize = getSize(self); 
	setSize(self, getSize(newTable));
	setSize(newTable, tempSize);

	List* tempList = getList(self);
	setList(self, getList(newTable));
	setList(newTable, tempList);

	// The strategy is we modify the table passed as argument
	// And free the new table with swapped memory locations
	delete(HashTable, newTable);

	return ok;
}
inline static Result remove(HashTable* self, size_t index)
{
	setup();
	run (List_Set(getList(self), index, DELETED));
	return ok;
}
inline static Result(ref) at(HashTable* self, size_t index)
{
	setup(ref);
	try (ref, r, List_At(getList(self), index));
	
	return OK(r);
}

// Constructs a HashTable of default size and returns the pointer
Result(ref) HashTable_Constructor(HashTable* self)
{
	return HashTable_Constructor1(self, 50);
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
	for (size_t i = 0; i < getSize(self); i++)
	{
		Result(ref) result = List_At(getList(self), i);
		if (result.code)
		{
			assert(OutOfBounds == result.code);
			continue;
		}

		if (result.value && DELETED != result.value)
		{
			delete(HashTableItem, result.value);
		}
	}
	delete(List, getList(self));
	return self;
}
// Inserts key, value returns self. If key exists, updates the value
Result HashTable_Upsert(HashTable* self, const Array* key, void* value)
{
	setup();

	if (70 < self->Count * 100 / getSize(self))
	{
		run (resize(self, getSize(self) * 2));
 	}

	// Search an empty slot location to insert the new item
	try (size_t, index, search(self, key, true));
	try (ref, r, at(self, index));
	HashTableItem* item = r;

	// If item exists, update. If not, create
	if (NULL == item || DELETED == item)
	{
		#warning TODO: object pool?
		try (ref, newPair, new2(HashTableItem, key, value));
		run (set(self, index, newPair));
	
		// Increase the count of items in the table
		self->Count++;
	}
	else
	{
		item->Value = value;
	}

	return ok;
}
// Deletes the key from the hash table
Result HashTable_Delete(HashTable* self, const Array* key)
{
	setup();
	try (size_t, index, search(self, key, false));
	try (ref, r, at(self, index));
	HashTableItem* item = r;
	
	// If search returned an empty slot location, we do nothing, this item doesn't exist in table already
	if (NULL == item)
	{
		return ok;
	}

	remove(self, index);
	delete(HashTableItem, item);

	self->Count--;

	if (10 > self->Count * 100 / getSize(self))
	{
		return resize(self, getSize(self) / 2);
	}

	return ok;
}
// Searches and retrieves the value from given key if exists
Result(ref) HashTable_Search(HashTable* self, const Array* key)
{
	// Search for the item with the given key
	setup(ref);
	try (size_t, index, search(self, key, false));
	try (ref, r, at(self, index));
	HashTableItem* item = r;

	if (NULL == item)
	{
		return OK(NULL);
	}

	// Return the value of the item found
	return (Result(ref)) {OK, item->Value};
}