#include <stdlib.h>

#include "List.h"

static List* resize(List* self, size_t size)
{
	Array* oldArray = self->_array;
	size_t oldCount = self->Count;
	
	$(!List_Constructor1(self, size));

	self->Count = oldCount;

	for (size_t i = 0; i < oldCount; i++)
	{
		void* oldItem = *(void**)Array_At(oldArray, i);
		Array_Set(self->_array, i, &oldItem);
	}

	delete(Array, oldArray);

	return self;
}

// Constructs a List of size and returns the list pointer, or NULL if any error
[[nodiscard]] List* List_Constructor1(List* self, size_t size)
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
List* List_Destructor(List* self)
{
	delete(Array, self->_array);
	return self;
}
// Returns the Item at given index; or NULL if out of bounds
[[nodiscard]] void* List_At(List* self, size_t index)
{
	$(index < 0 || self->Count <= index);
	return *(void**)Array_At(self->_array, index);
}
// Sets the Item at given index; or NULL if out of bounds
[[nodiscard]] void* List_Set(List* self, size_t index, const void* item)
{
	$(index < 0 || self->Count <= index);
	return Array_Set(self->_array, index, &item);
}
// Returns the first Item, or NULL if out of bounds
[[nodiscard]] void* List_Front(List* self)
{
	return List_At(self, 0);
}
// Returns the last Item, or NULL if out of bounds
[[nodiscard]] void* List_Back(List* self)
{
	return List_At(self, self->Count - 1);
}
// Inserts the Item at given index and returns it. Returns NULL if out of bounds, or any error
[[nodiscard]] void* List_Insert(List* self, size_t index, const void* item)
{
	$(index < 0 || self->Count < index);

	if (self->Count == self->_array->Count)
	{
		$(!resize(self, self->_array->Count * 2));
	}

	for (size_t i = self->Count; i > index; i--)
	{
		void* previous = List_At(self, i - 1);
		$(!previous);
		$(!Array_Set(self->_array, i, &previous));
	}

	$(!Array_Set(self->_array, index, &item));

	self->Count++;

	return List_At(self, index);
}
// Inserts the Item at front and returns it, or NULL if any error
[[nodiscard]] void* List_PushFront(List* self, const void* item)
{
	return List_Insert(self, 0, item);
}
// Inserts the Item at end and returns it, or NULL if any error
[[nodiscard]] void* List_PushBack(List* self, const void* item)
{
	return List_Insert(self, self->Count, item);
}
// Removes the Item at given index and returns removed. Returns NULL if out of bounds, or any error
[[nodiscard]] void* List_Remove(List* self, size_t index)
{
	$(!List_At(self, index));

	void* removed = List_At(self, index);

	for (size_t i = index; i < self->Count - 1; i++)
	{
		void* next = List_At(self, i + 1);
		$(!next);
		$(!Array_Set(self->_array, i, &next));
	}

	self->Count--;
	
	if (self->Count == self->_array->Count / 4)
	{
		$(!resize(self, self->_array->Count / 2));
	}

	return removed;
}
// Removes the Item at front and returns removed, or NULL if any error
void* List_RemoveFront(List* self)
{
	return List_Remove(self, 0);
}
// Removes the Item at end and returns removed, or NULL if any error
void* List_RemoveBack(List* self)
{
	return List_Remove(self, self->Count - 1);
}