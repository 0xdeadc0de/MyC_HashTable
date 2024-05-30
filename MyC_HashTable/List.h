#pragma once

#include <stddef.h>

#include "Array.h"

typedef struct List
{
	size_t Count;
	
	Array* _array;
}
List;

#include "List.h.gen"