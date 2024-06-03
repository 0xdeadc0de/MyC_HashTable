#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "HashTable2.h"
#include "HashTableItem2.h"

#include "HashTable2.head.c.gen"

TypedefResult(size_t);

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
static size_t polynomialHash(const Array* key, size_t size)
{
	// We're hashing ASCII strings, which has an alphabet size of 128 
	// We should choose a prime larger than that 151 should do
	return hash(key->Items, Array_Length(key), 151, size);
}
static Result(ref) search(HashTable2* self, const Array* key)
{
	size_t index = polynomialHash(key, getSize(self));

	setup(ref);
	try (ref, r, List_At(getList(self), index));
	HashTableItem2* item = r;

	// Search until we find the key matching or we find an empty slot
	while (NULL != item->Key && !Array_Equals(key, item->Key))
	{
		item = item->Next;
	}
	return OK(item);
}
static Result resize(HashTable2* self, size_t newSize)
{
	setup();
	if (newSize < 50)
	{
		return ok;
	}

	// Allocate a new table, and copy item pointers
	try (ref, r, new1(HashTable2, newSize));
	HashTable2* newTable = r;

	for (size_t i = 0; i < getSize(self); i++)
	{
		try (ref, r2, List_At(getList(self), i));
		HashTableItem2* item = r2;
		HashTableItem2* next;
		
		while (NULL != item->Key)
		{
			next = item->Next;
			HashTable2_Upsert(newTable, item->Key, item->Value);
			delete(HashTableItem2, item);
			item = next;
		}
		delete(HashTableItem2, item);
		run (List_Set(getList(self), i, NULL));
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
	delete(HashTable2, newTable);

	return ok;
}
// Constructs a HashTable of default size and returns the pointer
Result(ref) HashTable2_Constructor(HashTable2* self)
{
	return HashTable2_Constructor1(self, 50);
}
// Constructs a HashTable and returns the pointer
Result(ref) HashTable2_Constructor1(HashTable2* self, size_t size)
{
	size = findNextPrime(size);

	setup(ref);
	try (ref, r, new1(List, size));
	List* list = r;

	*self = (HashTable2)
	{
		.Count = 0,
		
		._size = size,
		._list = list
	};
	
	for (size_t i = 0; i < size; i++)
	{
		try (ref, item, new(HashTableItem2));
		run (List_PushBack(getList(self), item));
	}

	return (Result(ref)) {OK, self};
}
// Frees the resources held, and returns reference to self
HashTable2* HashTable2_Destructor(HashTable2* self)
{
	for (size_t i = 0; i < getSize(self); i++)
	{
		Result(ref) result = List_At(getList(self), i);
		if (result.code)
		{
			assert(OutOfBounds == result.code);
			continue;
		}

		HashTableItem2* item = result.value;
		HashTableItem2* next;
		while (item)
		{
			next = item->Next;
			delete(HashTableItem2, item);
			item = next;
		}
	}
	delete(List, getList(self));
	return self;
}
// Inserts key, value returns self. If key exists, updates the value
Result HashTable2_Upsert(HashTable2* self, const Array* key, void* value)
{
	setup();

	if (70 < self->Count * 100 / getSize(self))
	{
		run (resize(self, getSize(self) * 2));
 	}

	// Search an empty slot location to insert the new item
	try (ref, r, search(self, key));
	HashTableItem2* item = r;

	// If item exists, update. If not, create
	if (NULL == item->Key)
	{
		#warning TODO: object pool?
		try (ref, cloned, Array_Clone(key));
		item->Key = cloned;
		item->Value = value;

		try (ref, r2, new(HashTableItem2));
		HashTableItem2* next = r2;
		item->Next = next;
		next->Prev = item;
	
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
Result HashTable2_Delete(HashTable2* self, const Array* key)
{
	setup();
	try (ref, r, search(self, key));
	HashTableItem2* item = r;
	
	// If search returned an empty slot location, we do nothing, this item doesn't exist in table already
	if (NULL == item->Key)
	{
		return ok;
	}

	HashTableItem2* itemNext = item->Next;
	HashTableItem2* itemPrev = item->Prev;
	
	// If this is head node, we will shift, and pop the next node in chain
	if (NULL == itemPrev)
	{
		// Copy all from next into this item
		item->Next = itemNext->Next;
		item->Value = itemNext->Value;

		// Swap keys so correct key will be free'd on delete
		void* temp = item->Key;
		item->Key = itemNext->Key;
		itemNext->Key = temp;

		// Set next node to point to this item
		if (itemNext->Next)
		{
			itemNext->Next->Prev = item;
		}
		delete(HashTableItem2, itemNext);
	}
	else
	{
		// This is either mid or tail node, we will pop this, and join two nodes
		itemPrev->Next = itemNext;
		itemNext->Prev = itemPrev;
		delete(HashTableItem2, item);
	}

	self->Count--;

	if (10 > self->Count * 100 / getSize(self))
	{
		return resize(self, getSize(self) / 2);
	}

	return ok;
}
// Searches and retrieves the value from given key if exists
Result(ref) HashTable2_Search(HashTable2* self, const Array* key)
{
	// Search for the item with the given key
	setup(ref);
	try (ref, r, search(self, key));
	HashTableItem2* item = r;

	if (NULL == item->Key)
	{
		return OK(NULL);
	}

	// Return the value of the item found
	return OK(item->Value);
}