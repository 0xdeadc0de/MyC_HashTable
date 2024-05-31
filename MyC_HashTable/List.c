#include <stdlib.h>

#include "List.h"

// Resizes the List (modifies given pointer), returns self
[[nodiscard]] static Resultref resize(List* self, size_t size)
{
	Array* oldArray = self->_array;
	size_t oldCount = self->Count;
	
	try_old (List_Constructor1(self, size))
	end_old 

	self->Count = oldCount;

	for (size_t i = 0; i < oldCount; i++)
	{
		void* oldItem; 
		try_old (Array_At(oldArray, i))
		out_old (oldItem)

		try_old (Array_Set(self->_array, i, oldItem))
		end_old
	}

	delete(Array, oldArray);

	return (Resultref) {OK, self};
}

// Constructs a List of size and returns the self pointer
[[nodiscard]] Resultref List_Constructor1(List* self, size_t size)
{
	if (size <= 0)
	{
		return (Resultref) {InvalidArgument};
	}

	Array* array;
	try_old (new2(Array, sizeof(void*), size))
	out_old (array)

	*self = (List)
	{
		.Count = 0,
		._array = array
	};

	return (Resultref) {OK, self};
}
// Frees the resources held, and returns reference to self
List* List_Destructor(List* self)
{
	delete(Array, self->_array);
	return self;
}
// Returns the item at given index; or NULL if out_old of bounds
Resultref List_At(List* self, size_t index)
{
	if (index < 0 || self->Count <= index)
	{
		return (Resultref) {out_oldOfBounds};
	}

	void* location;
	try_old (Array_At(self->_array, index))
	out_old (location);

	return (Resultref) {OK, *(void**)location};
}
// Sets the Item at given index
Resultref List_Set(List* self, size_t index, const void* item)
{
	if (index < 0 || self->Count <= index)
	{
		return (Resultref) {out_oldOfBounds};
	}

	return Array_Set(self->_array, index, &item);
}
// Returns the first Item
Resultref List_Front(List* self)
{
	return List_At(self, 0);
}
// Returns the last Item
Resultref List_Back(List* self)
{
	return List_At(self, self->Count - 1);
}
// Inserts the Item at given index and returns it
Resultref List_Insert(List* self, size_t index, const void* item)
{
	if (index < 0 || self->Count < index)
	{
		return (Resultref) {out_oldOfBounds};
	}

	if (self->Count == self->_array->Count)
	{
		try_old (resize(self, self->_array->Count * 2))
		end_old
	}

	for (size_t i = self->Count; i > index; i--)
	{
		void* previous;
		try_old (List_At(self, i - 1))
		out_old (previous)

		try_old (Array_Set(self->_array, i, &previous))
		end_old
	}

	try_old (Array_Set(self->_array, index, &item))
	end_old

	self->Count++;

	return List_At(self, index);
}
// Inserts the Item at front and returns it, or NULL if any error
Resultref List_PushFront(List* self, const void* item)
{
	return List_Insert(self, 0, item);
}
// Inserts the Item at end and returns it, or NULL if any error
Resultref List_PushBack(List* self, const void* item)
{
	return List_Insert(self, self->Count, item);
}
// Removes the Item at given index and returns removed. Returns NULL if out_old of bounds, or any error
Resultref List_Remove(List* self, size_t index)
{
	try_old (List_At(self, index))
	end_old

	void* removed;
	try_old (List_At(self, index))
	out_old (removed)

	for (size_t i = index; i < self->Count - 1; i++)
	{
		void* next;
		try_old (List_At(self, i + 1))
		out_old (next)
		
		try_old (Array_Set(self->_array, i, &next))
		end_old
	}

	self->Count--;
	
	if (self->Count == self->_array->Count / 4)
	{
		try_old (resize(self, self->_array->Count / 2))
		end_old
	}

	return (Resultref) {OK, removed};
}
// Removes the Item at front and returns removed, or NULL if any error
Resultref List_RemoveFront(List* self)
{
	return List_Remove(self, 0);
}
// Removes the Item at end and returns removed, or NULL if any error
Resultref List_RemoveBack(List* self)
{
	return List_Remove(self, self->Count - 1);
}