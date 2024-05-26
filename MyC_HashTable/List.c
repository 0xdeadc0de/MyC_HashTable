#include <stdlib.h>

#include "List.h"

static List* resize(List* self, size_t size)
{
	void** oldItems = self->_items;
	size_t oldCount = self->Count;
	
	$(!$List.Constructor(self, size));

	self->Count = oldCount;

	for (size_t i = 0; i < oldCount; i++)
	{
		self->_items[i] = oldItems[i];
	}

	free(oldItems);

	return self;
}

// Constructs a List of size and returns the list pointer, or NULL if any error
List* Constructor(List* self, size_t size)
{
	$(size <= 0);

	*self = (List)
	{
		.Count = 0,
		._size = size,
		._items = calloc(sizeof(void*), size)
	};

	$(!self->_items);

	return self;
}
void Destructor(List* self)
{
	free(self->_items);
}
// Returns the Item at given index; or NULL if out of bounds
void* At(List* self, size_t index)
{
	$(index < 0 || self->Count <= index);
	return self->_items[index];
}
// Returns the first Item, or NULL if out of bounds
void* Front(List* self)
{
	return $List.At(self, 0);
}
// Returns the last Item, or NULL if out of bounds
void* Back(List* self)
{
	return $List.At(self, self->Count - 1);
}
// Inserts the Item at given index. Returns NULL if out of bounds, or any error
void* Insert(List* self, size_t index, void* item)
{
	$(index < 0 || self->Count < index);

	if (self->Count == self->_size)
	{
		$(!resize(self, self->_size * 2));
	}

	for (size_t i = self->Count; i > index; i--)
	{
		self->_items[i] = self->_items[i - 1];
	}
	
	self->_items[index] = item;

	self->Count++;

	return self->_items[index];
}
// Inserts the Item at front, or NULL if any error
void* PushFront(List* self, void* item)
{
	return Insert(self, 0, item);
}
// Inserts the Item at end, or NULL if any error
void* PushBack(List* self, void* item)
{
	return Insert(self, self->Count, item);
}
// Removes the Item at given index. Returns NULL if out of bounds, or any error
void* Remove(List* self, size_t index)
{
	$(!$List.At(self, index));

	void* removed = self->_items[index];

	for (size_t i = index; i < self->Count - 1; i++)
	{
		self->_items[i] = self->_items[i + 1];
	}

	self->Count--;
	
	if (self->Count == self->_size / 4)
	{
		$(!resize(self, self->_size / 2));
	}

	return removed;
}
// Removes the Item at front, or NULL if any error
void* RemoveFront(List* self)
{
	return Remove(self, 0);
}
// Removes the Item at end, or NULL if any error
void* RemoveBack(List* self)
{
	return Remove(self, self->Count - 1);
}

#include "List.c.gen"