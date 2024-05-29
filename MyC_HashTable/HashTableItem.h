#pragma once

#include "Array.h"

typedef struct [[nodiscard]]
{
	Array* Key;
	void* Value;
}
HashTableItem;

#include "HashTableItem.h.gen"