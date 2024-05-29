#pragma once

#include "Array.h"

typedef struct
{
	Array* Key;
	void* Value;
}
HashTableItem;

#include "HashTableItem.h.gen"