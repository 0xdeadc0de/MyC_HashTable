#pragma once

#include <stddef.h>

#include "List.h"

typedef struct HashTable2
{
	size_t Count;

	[[deprecated("private")]] size_t _size;
	[[deprecated("private")]] List* _list;
}
HashTable2;

#include "HashTable2.h.gen"