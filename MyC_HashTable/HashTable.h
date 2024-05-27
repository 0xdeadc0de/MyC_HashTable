#pragma once

#include <stddef.h>

#include "List.h"

typedef struct
{
	size_t Count;

	size_t _size;
	List* _list;
}
HashTable;

#include "HashTable.h.gen"