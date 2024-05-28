#include <stdlib.h>

#include "List.h"

static List* resize(List* self, size_t size)
{
	Array* oldArray = self->_array;
	size_t oldCount = self->Count;
	
	$(!$List.Constructor(self, size));

	self->Count = oldCount;

	for (size_t i = 0; i < oldCount; i++)
	{
		void* oldItem = *(void**)$Array.At(oldArray, i);
		$Array.Set(self->_array, i, &oldItem);
	}

	delete(Array, oldArray);

	return self;
}

// Constructs a List of size and returns the list pointer, or NULL if any error
static List* Constructor(List* self, size_t size)
{
	$(size <= 0);

	*self = (List)
	{
		.Count = 0,
		._array = new2(Array, sizeof(void*), size)
	};
	
	$(!self->_array);

	return self;
}
// Frees the resources held, and returns reference to self
static void* Destructor(List* self)
{
	delete(Array, self->_array);
	return self;
}
// Returns the Item at given index; or NULL if out of bounds
static void* At(List* self, size_t index)
{
	$(index < 0 || self->Count <= index);
	return *(void**)$Array.At(self->_array, index);
}
// Sets the Item at given index; or NULL if out of bounds
static void* Set(List* self, size_t index, const void* item)
{
	$(index < 0 || self->Count <= index);
	return $Array.Set(self->_array, index, &item);
}
// Returns the first Item, or NULL if out of bounds
static void* Front(List* self)
{
	return $List.At(self, 0);
}
// Returns the last Item, or NULL if out of bounds
static void* Back(List* self)
{
	return $List.At(self, self->Count - 1);
}
// Inserts the Item at given index. Returns NULL if out of bounds, or any error
static void* Insert(List* self, size_t index, const void* item)
{
	$(index < 0 || self->Count < index);

	if (self->Count == self->_array->Length)
	{
		$(!resize(self, self->_array->Length * 2));
	}

	for (size_t i = self->Count; i > index; i--)
	{
		void* previous = $List.At(self, i - 1);
		$Array.Set(self->_array, i, &previous);
	}

	$Array.Set(self->_array, index, &item);

	self->Count++;

	return *(void**)$Array.At(self->_array, index);
}
// Inserts the Item at front, or NULL if any error
static void* PushFront(List* self, const void* item)
{
	return Insert(self, 0, item);
}
// Inserts the Item at end, or NULL if any error
static void* PushBack(List* self, const void* item)
{
	return Insert(self, self->Count, item);
}
// Removes the Item at given index. Returns NULL if out of bounds, or any error
static void* Remove(List* self, size_t index)
{
	$(!$List.At(self, index));

	void* removed = $List.At(self, index);

	for (size_t i = index; i < self->Count - 1; i++)
	{
		void* next = $List.At(self, i + 1);
		$Array.Set(self->_array, i, &next);
	}

	self->Count--;
	
	if (self->Count == self->_array->Length / 4)
	{
		$(!resize(self, self->_array->Length / 2));
	}

	return removed;
}
// Removes the Item at front, or NULL if any error
static void* RemoveFront(List* self)
{
	return Remove(self, 0);
}
// Removes the Item at end, or NULL if any error
static void* RemoveBack(List* self)
{
	return Remove(self, self->Count - 1);
}

#include "List.c.gen"