#include <stdlib.h>
#include <string.h>

#include "HashTableItem.h"

// Constructs an HashTableItem with cloned key and returns the pointer, or NULL if any error
[[nodiscard]] Result(ref) HashTableItem_Constructor2(HashTableItem* self, const Array* key, void* value)
{
	ret (ref);
	try (ref, clone, Array_Clone(key));

	self->Key = clone;
	self->Value = value;

	return (Result(ref)) {OK, self};
}
// Frees the resources held, and returns reference to self
HashTableItem* HashTableItem_Destructor(HashTableItem* self)
{
	delete(Array, self->Key);
	return self;
}