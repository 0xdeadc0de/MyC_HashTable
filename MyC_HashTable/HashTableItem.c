#include <stdlib.h>
#include <string.h>

#include "HashTableItem.h"

// Constructs an HashTableItem with cloned key and returns the pointer, or NULL if any error
static HashTableItem* Constructor(HashTableItem* self, const Array* key, const void* value)
{
	(Array*)memcpy(malloc(sizeof(Array)), key, sizeof(Array));
}

#include "HashTableItem.c.gen"