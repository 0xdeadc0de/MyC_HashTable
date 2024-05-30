#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct Array
{
	size_t Count;
	size_t SizeOfItem;
	void* Items;
}
Array;

#include "Array.h.gen"