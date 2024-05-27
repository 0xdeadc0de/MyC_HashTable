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
/*
// Constructs a HashTable and returns the pointer, or NULL if any error
static HashTable* Constructor(HashTable* self, size_t size)
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

static Item* allocItem(const char* key, const char* value)
{
	Item* item = malloc(sizeof(Item));
	if (NULL == item)
	{
		return NULL;
	}

	item->key   = _strdup(key);
	item->value = _strdup(value);
	
	return item;
}
static void freeItem(Item* item)
{
	if (item == NULL || item == DELETED)
	{
		return;
	}
	free(item->key);
	free(item->value);
	free(item);
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
		Item* item = table->items[i];
		
		// If an item is deleted or null, skip it
		if (NULL == item || DELETED == item)
		{
			continue;
		}

		Insert(newTable, item->key, item->value);
	}

	// Copy the final amount of count to new table
	table->count = newTable->count;

	// Swap tables so we can free memory on new table pointer
	int tempSize = table->size; 
	table->size = newTable->size;
	newTable->size = tempSize;

	Item** tempItems = table->items;
	table->items = newTable->items;
	newTable->items = tempItems;

	// The strategy is we modify the table passed as argument
	// And free the new table with swapped memory locations
	FreeTable(newTable);
}

// Public methods
Table* AllocTable()
{
	return allocTable(50);
}
void FreeTable(Table* table)
{
	for (int i = 0; i < table->size; i++)
	{
		Item* item = table->items[i];
		freeItem(item);
	}

	free(table->items);
	free(table);
}
void Insert(HashTable* self, const Array* key, const void* value)
{
	if (70 < self->Count * 100 / self->_size)
	{
		resize(table, table->size * 2);
 	}

	// Search an empty slot location to insert the new item
	size_t index = search(self, key, true);
	$List.Set(self->_list, index, )

	// Increase the count of items in the table
	self->Count++;
}
void Delete(Table* table, const char* key)
{
	Item** itemLocation = search(table, key, false);
	Item*  item = *itemLocation;

	// If search returned an empty slot location, we do nothing, this item doesn't exist in table already
	if (NULL == item)
	{
		return;
	}

	*itemLocation = DELETED;

	table->count--;

	if (10 > table->count * 100 / table->size)
	{
		resize(table, table->size / 2);
	}
}
char* Search(Table* table, const char* key)
{
	// Search for the item with the given key
	Item** itemLocation = search(table, key, false);
	Item*  item = *itemLocation;
	if (NULL == item)
	{
		return NULL;
	}

	// Return the value of the item found
	return item->value;
}

#include "HashTable.c.gen"
*/