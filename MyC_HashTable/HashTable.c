#include "HashTableItem.h"

typedef struct
{
	int count;

	int _size;
	HashTableItem** _items;
}
HashTable;