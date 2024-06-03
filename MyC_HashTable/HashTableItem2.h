#pragma once

#include "Array.h"

typedef struct HashTableItem2
{
	Array* Key;
	void* Value;
	struct HashTableItem2* Prev;
	struct HashTableItem2* Next;
}
HashTableItem2;

#include "HashTableItem2.h.gen"