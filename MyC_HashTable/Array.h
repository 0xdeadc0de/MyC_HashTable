#pragma once

#include <stddef.h>

typedef struct
{
	size_t Length;
	size_t SizeOfItem;
	void* Items;
}
Array;

#include "Array.h.gen"