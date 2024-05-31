#pragma once

#include <stddef.h>

#include "List.h"

typedef struct HashTable
{
	size_t Count;

	[[deprecated("private")]] size_t _size;
	[[deprecated("private")]] List* _list;
}
HashTable;

#include "HashTable.h.gen"