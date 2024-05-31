#include <stdlib.h>
#include <string.h>

#include "HashTableItem.h"

// Constructs an HashTableItem with cloned key and returns the pointer, or NULL if any error
[[nodiscard]] Result HashTableItem_Constructor2(HashTableItem* self, const Array* key, void* value)
{
	Array* clone;
	try_old (Array_Clone(key))
	out_old (clone)

	self->Key = clone;
	self->Value = value;

	return (Result) {OK, self};
}
// Frees the resources held, and returns reference to self
HashTableItem* HashTableItem_Destructor(HashTableItem* self)
{
	delete(Array, self->Key);
	return self;
}