#pragma once

#include "Array.h"

typedef struct HashTableItem
{
	Array* Key;
	void* Value;
}
HashTableItem;

#include "HashTableItem.h.gen"