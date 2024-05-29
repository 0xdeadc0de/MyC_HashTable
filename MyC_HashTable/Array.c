#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Array.h"

// Constructs an Array of size and returns the array pointer, or NULL if any error
static Array* Constructor2(Array* self, size_t sizeOfItem, size_t length)
{
	$(sizeOfItem <= 0 || length <= 0);

	*self = (Array)
	{
		.Count = length,
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
static Array* Clone(const Array* self)
{
	Array* clone = malloc(sizeof(Array));
	$(!clone);
	clone = (Array*)memcpy(clone, self, sizeof(Array));

	void* clonedItems = malloc($Array.Length(self));
	$(!clonedItems);
	clonedItems = memcpy(clonedItems, self->Items, $Array.Length(self));

	clone->Items = clonedItems;

	return clone;
}
// Returns the location of the item at index to read from, or NULL if any error
static void* At(const Array* self, size_t index)
{
	$(self->Count <= index);

	const size_t offset = self->SizeOfItem * index;
	char* baseAddress = ((char*)self->Items);

	return &baseAddress[offset];
}
// Copies the item at the given location to the index 
// and returns internal location of item, or NULL if any error
static void* Set(Array* self, size_t index, const void* itemLocation)
{
	$(self->Count <= index);
	
	const size_t offset = self->SizeOfItem * index;
	char* baseAddress = ((char*)self->Items);

	memcpy(&baseAddress[offset], itemLocation, self->SizeOfItem);

	return &baseAddress[offset];
}
// Compares two Arrays, returns true if they're exactly identical
static bool Equals(const Array* self, const Array* other)
{
	if (self->Count != other->Count || self->SizeOfItem != other->SizeOfItem)
	{
		return false;
	}

	return !memcmp(self->Items, other->Items, $Array.Length(self));
}
// Returns the total size of the Array in bytes, calculated by SizeOfItem * Count
static size_t Length(const Array* self)
{
	return self->Count * self->SizeOfItem;
}

#include "Array.c.gen"