#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Array.h"

// Constructs an Array of size and returns the array pointer
[[nodiscard]] Resultref Array_Constructor2(Array* self, size_t sizeOfItem, size_t length)
{
	if (sizeOfItem <= 0 || length <= 0)
	{
		return (Resultref) {InvalidArgument};
	}

	*self = (Array)
	{
		.Count = length,
		.SizeOfItem = sizeOfItem,
		.Items = calloc(sizeOfItem, length)
	};

	if (NULL == self->Items)
	{
		return (Resultref) {AllocNull};
	}

	return (Resultref) {OK, self};
}
// Frees the resources held, and returns reference to self
Array* Array_Destructor(Array* self)
{
	free(self->Items);
	return self;
}
// Creates an exact copy and returns the pointer
[[nodiscard]] Resultref Array_Clone(const Array* self)
{
	Array* clone = malloc(sizeof(Array));
	if (NULL == clone)
	{
		return (Resultref) {AllocNull};
	}

	clone = (Array*)memcpy(clone, self, sizeof(Array));

	void* clonedItems = malloc(Array_Length(self));
	if (NULL == clonedItems)
	{
		return (Resultref) {AllocNull};
	}

	clonedItems = memcpy(clonedItems, self->Items, Array_Length(self));

	clone->Items = clonedItems;

	return (Resultref) {OK, clone};
}
// Returns the location of the item at index to read from
[[nodiscard]] Resultref Array_At(const Array* self, size_t index)
{
	if (self->Count <= index)
	{
		return (Resultref) {OutOfBounds};
	}

	const size_t offset = self->SizeOfItem * index;
	char* baseAddress = ((char*)self->Items);

	return (Resultref) {OK, &baseAddress[offset]};
}
// Copies the item at the given location to the index and returns internal location of item
[[nodiscard]] Resultref Array_Set(Array* self, size_t index, const void* itemLocation)
{
	if (self->Count <= index)
	{
		return (Resultref) {OutOfBounds};
	}
	
	const size_t offset = self->SizeOfItem * index;
	char* baseAddress = ((char*)self->Items);

	memcpy(&baseAddress[offset], itemLocation, self->SizeOfItem);

	return (Resultref) {OK, &baseAddress[offset]};
}
// Compares two Arrays, returns true if they're exactly identical
bool Array_Equals(const Array* self, const Array* other)
{
	if (self->Count != other->Count || self->SizeOfItem != other->SizeOfItem)
	{
		return false;
	}

	return !memcmp(self->Items, other->Items, Array_Length(self));
}
// Returns the total size of the Array in bytes, calculated by SizeOfItem * Count
size_t Array_Length(const Array* self)
{
	return self->Count * self->SizeOfItem;
}