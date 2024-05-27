#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
	size_t Length;
	size_t SizeOfItem;
	void* Items;
}
Array;

#include "Array.h.gen"