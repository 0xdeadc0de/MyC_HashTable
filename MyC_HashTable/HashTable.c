#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "HashTable.h"
#include "HashTableItem.h"

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
static Result search(HashTable* self, const Array* key, bool findInsertLocation)
{
	size_t attempt = 0;
	// Search until we find the key matching or we find an empty slot
	for (;;)
	{
		size_t index = doubleHash(key, self->_size, attempt);
		attempt++;

		HashTableItem* item;
		try (List_At(self->_list, index))
		out (item)

		if (NULL == item)
		{
			return (Result) {OK, index};
		}

		if (DELETED == item)
		{
			if (findInsertLocation)
			{
				return (Result) {OK, index};
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
		
		if (Array_Equals(key, item->Key))
		{
			return (Result) {OK, index};;
		}
	}
}
static Result resize(HashTable* self, size_t newSize)
{
	if (newSize < 50)
	{
		return (Result) {OK, self};
	}

	// Allocate a new table, and copy item pointers
	HashTable* newTable;
	try (new1(HashTable, newSize))
	out (newTable);

	for (size_t i = 0; i < self->_size; i++)
	{
		HashTableItem* item;
		try (List_At(self->_list, i))
		out (item)
		
		// If an item is deleted or null, skip it
		if (NULL == item || DELETED == item)
		{
			continue;
		}

		try (HashTable_Upsert(newTable, item->Key, item->Value))
		end
	}

	// Swap tables so we can free memory on new table pointer
	size_t tempSize = self->_size; 
	self->_size = newTable->_size;
	newTable->_size = tempSize;

	List* tempList = self->_list;
	self->_list = newTable->_list;
	newTable->_list = tempList;

	// The strategy is we modify the table passed as argument
	// And free the new table with swapped memory locations
	delete(HashTable, newTable);

	return (Result) {OK, self};
}

// Constructs a HashTable and returns the pointer, or NULL if any error
[[nodiscard]] Result HashTable_Constructor1(HashTable* self, size_t size)
{
	size = findNextPrime(size);

	List* list;
	try (new1(List, size))
	out (list)

	*self = (HashTable)
	{
		.Count = 0,
		
		._size = size,
		._list = list
	};
	
	for (size_t i = 0; i < size; i++)
	{
		List_PushBack(self->_list, NULL);
	}

	return (Result) {OK, self};
}
// Frees the resources held, and returns reference to self
HashTable* HashTable_Destructor(HashTable* self)
{
	delete(List, self->_list);
	return self;
}
// Inserts key, value returns self. If key exists, updates the value. Returns NULL if any error
[[nodiscard]] Result HashTable_Upsert(HashTable* self, const Array* key, void* value)
{
	if (70 < self->Count * 100 / self->_size)
	{
		try (resize(self, self->_size * 2))
		end
 	}

	// Search an empty slot location to insert the new item
	size_t index;
	try (search(self, key, true))
	out (index)

	HashTableItem* newPair;
	try (new2(HashTableItem, key, value))
	out (newPair)

	// Set item
	try (List_Set(self->_list, index, newPair))
	end

	// Increase the count of items in the table
	self->Count++;

	return (Result) {OK, self};
}
// Deletes the key from the hash table and returns self
[[nodiscard]] Result HashTable_Delete(HashTable* self, const Array* key)
{
	size_t index;
	try (search(self, key, false))
	out (index)
	
	HashTableItem* item;
	try (List_At(self->_list, index))
	out (item)
	
	// If search returned an empty slot location, we do nothing, this item doesn't exist in table already
	if (NULL == item)
	{
		return (Result) {OK, self};
	}

	List_Set(self->_list, index, DELETED);
	delete(HashTableItem, item);

	self->Count--;

	if (10 > self->Count * 100 / self->_size)
	{
		return resize(self, self->_size / 2);
	}

	return (Result) {OK, self};
}
// Searches and retrieves the value from given key if exists
[[nodiscard]] Result HashTable_Search(HashTable* self, const Array* key)
{
	// Search for the item with the given key
	size_t index;
	try (search(self, key, false))
	out (index)

	HashTableItem* item;
	try (List_At(self->_list, index))
	out (item)

	// Return the value of the item found
	return (Result) {OK, item->Value};
}