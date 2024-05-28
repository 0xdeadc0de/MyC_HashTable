#include <stdlib.h>
#include <string.h>

#include "HashTableItem.h"

// Constructs an HashTableItem with cloned key and returns the pointer, or NULL if any error
static HashTableItem* Constructor2(HashTableItem* self, const Array* key, void* value)
{
	self->Key = $Array.Clone(key);
	self->Value = value;

	return self;
}
// Frees the resources held, and returns reference to self
static void* Destructor(HashTableItem* self)
{
	delete(Array, self->Key);
	return self;
}

#include "HashTableItem.c.gen"