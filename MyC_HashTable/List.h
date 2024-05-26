#pragma once

#include <stddef.h>

typedef struct
{
	size_t Count;
	
	void** _items;
	size_t _size;
}
List;

#include "List.h.gen"