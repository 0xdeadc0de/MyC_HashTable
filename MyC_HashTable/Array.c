#include <stdlib.h>

#include "Array.h"

// Constructs an Array of size and returns the array pointer, or NULL if any error
static Array* Constructor(Array* self, size_t sizeOfItem, size_t length)
{
	$(sizeOfItem <= 0 || length <= 0);

	*self = (Array)
	{
		.Length = length,
		.SizeOfItem = sizeOfItem,
		.Items = calloc(sizeOfItem, length)
	};

	$(!self->Items);

	return self;
}
// Frees the resources held, and returns reference to self
static void* Destructor(Array* self)
{
	free(self->Items);
	return self;
}
// Returns the location of the item at index to read from, or NULL if any error
static void* At(Array* self, size_t index)
{
	$(index < 0 || self->Length <= index);

	const size_t offset = self->SizeOfItem * index;
	char* baseAddress = ((char*)self->Items);

	return &baseAddress[offset];
}
// Copies the item at the given location to the index 
// and returns internal location of item, or NULL if any error
static void* Set(Array* self, size_t index, void* itemLocation)
{
	$(index < 0 || self->Length <= index);
	
	const size_t offset = self->SizeOfItem * index;
	char* baseAddress = ((char*)self->Items);

	for (int i = 0; i < self->SizeOfItem; i++)
	{
		char byte = ((char*)itemLocation)[i];
		baseAddress[offset + i] = byte;
	}

	return baseAddress[offset];
}

#include "Array.c.gen"