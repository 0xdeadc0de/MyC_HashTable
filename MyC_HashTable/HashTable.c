#include <stddef.h>

#include "HashTableItem.h"

typedef struct
{
	size_t Count;

	size_t _size;
	HashTableItem** _items;
}
HashTable;