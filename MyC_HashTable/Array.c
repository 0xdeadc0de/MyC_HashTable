#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Array.h"
_Check_return_
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
// Creates an exact copy and returns the pointer, or NULL if any error
static Array* Clone(Array* self)
{
	Array* clone = malloc(sizeof(Array));
	$(!clone);
	clone = (Array*)memcpy(clone, self, sizeof(Array));

	size_t size = self->Length * self->SizeOfItem;
	void* cloneItems = malloc(size);
	$(!cloneItems);
	cloneItems = memcpy(cloneItems, self->Items, size);

	clone->Items = cloneItems;

	return clone;
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
static void* Set(Array* self, size_t index, const void* itemLocation)
{
	$(index < 0 || self->Length <= index);
	
	const size_t offset = self->SizeOfItem * index;
	char* baseAddress = ((char*)self->Items);

	for (int i = 0; i < self->SizeOfItem; i++)
	{
		char byte = ((char*)itemLocation)[i];
		baseAddress[offset + i] = byte;
	}

	return (void*)baseAddress[offset];
}
// Compares two Arrays, returns true if they're exactly identical
static bool Equals(Array* self, Array* other)
{
	if (self->Length != other->Length || self->SizeOfItem != other->SizeOfItem)
	{
		return false;
	}

	for (size_t i = 0; i < self->Length * self->SizeOfItem; i++)
	{
		if (((char*)self->Items)[i] != ((char*)other->Items)[i])
		{
			return false;
		}
	}

	return true;
}

#include "Array.c.gen"