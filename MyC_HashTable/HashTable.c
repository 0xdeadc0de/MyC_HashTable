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
	for (int i = 3; i <= floor(sqrt((double)x)); i += 2)
	{
		if (0 == (x % i))
		{
			i = 1;
			x += 2;
		}
	}

	return x;
}
// Polynomial rolling hash function
static int hash(const char* key, size_t n, int prime, size_t size)
{
	int hash = 0;
	for (size_t i = 0; i < n; i++)
	{
		hash += ((unsigned)pow(prime, (double)i) % size) * (key[i] % size);
		hash %= size;
	}

	return hash;
}
// Open addressing, we will use double hashing
static int doubleHash(const char* key, size_t n, size_t size, int attempt)
{
	// We're hashing ASCII strings, which has an alphabet size of 128 
	// We should choose a prime larger than that 151, 163 should do
	const int firstHash  = hash(key, n, 151, size);
	const int secondHash = hash(key, n, 163, size-1) + 1;

	int finalHash = firstHash + attempt * secondHash;
	finalHash %= size;

	return finalHash;
}
static size_t search(HashTable* self, const Array* key, bool findInsertLocation)
{
	int attempt = 0;
	// Search until we find the key matching or we find an empty slot
	for (;;)
	{
		int index = doubleHash(key->Items, key->Length * key->SizeOfItem, self->_size, attempt);
		attempt++;

		HashTableItem* item = $List.At(self->_list, index);

		if (NULL == item)
		{
			return index;
		}

		if (DELETED == item)
		{
			if (findInsertLocation)
			{
				return index;
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
		
		if ($Array.Equals(key, item->Key))
		{
			return index;
		}
	}
}
extern void Insert(HashTable* self, const Array* key, const void* value);
static void resize(HashTable* self, size_t newSize)
{
	if (newSize < 50)
	{
		return;
	}

	// Allocate a new table, and copy item pointers
	HashTable* newTable = new1(HashTable, newSize);
	for (size_t i = 0; i < self->_size; i++)
	{
		HashTableItem* item = $List.At(self->_list, i);
		
		// If an item is deleted or null, skip it
		if (NULL == item || DELETED == item)
		{
			continue;
		}

		Insert(newTable, item->Key, item->Value);
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
}

// Constructs a HashTable and returns the pointer, or NULL if any error
static HashTable* Constructor1(HashTable* self, size_t size)
{
	size = findNextPrime(size);

	*self = (HashTable)
	{
		.Count = 0,
		
		._size = size,
		._list = new1(List, size)
	};
	
	for (size_t i = 0; i < size; i++)
	{
		$List.PushBack(self->_list, NULL);
	}

	return self;
}
// Frees the resources held, and returns reference to self
static void* Destructor(HashTable* self)
{
	delete(List, self->_list);
	return self;
}
// Inserts the given key, value into the hash table
static void Insert(HashTable* self, const Array* key, const void* value)
{
	if (70 < self->Count * 100 / self->_size)
	{
		resize(self, self->_size * 2);
 	}

	// Search an empty slot location to insert the new item
	size_t index = search(self, key, true);
	HashTableItem* newPair = new2(HashTableItem, key, value);
	$List.Set(self->_list, index, newPair);

	// Increase the count of items in the table
	self->Count++;
}
// Deletes the key from the hash table
static void Delete(HashTable* self, const Array* key)
{
	size_t index = search(self, key, false);
	HashTableItem* item = $List.At(self->_list, index);
	// If search returned an empty slot location, we do nothing, this item doesn't exist in table already
	if (NULL == item)
	{
		return;
	}

	$List.Set(self->_list, index, DELETED);
	delete(HashTableItem, item);

	self->Count--;

	if (10 > self->Count * 100 / self->_size)
	{
		resize(self, self->_size / 2);
	}
}
// Searches and retrieves the value from given key if exists, or NULL if any error
static void* Search(HashTable* self, const Array* key)
{
	// Search for the item with the given key
	size_t index = search(self, key, false);
	HashTableItem* item = $List.At(self->_list, index);
	$(!item);

	// Return the value of the item found
	return item->Value;
}

#include "HashTable.c.gen"