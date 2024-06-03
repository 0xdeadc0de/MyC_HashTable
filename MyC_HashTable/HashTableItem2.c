#include <stdlib.h>
#include <string.h>

#include "HashTableItem2.h"

// Constructs an HashTableItem2 with cloned key and returns the pointer
Result(ref) HashTableItem2_Constructor(HashTableItem2* self)
{
	setup(ref);

	self->Key = NULL;
	self->Value = NULL;
	self->Next = NULL;
	self->Prev = NULL;
	
	return OK(self);
}
// Frees the resources held, and returns reference to self
HashTableItem2* HashTableItem2_Destructor(HashTableItem2* self)
{
	if (self->Key)
	{
		delete(Array, self->Key);
	}
	return self;
}