#include <stdlib.h>

#include "List.h"

// Resizes the List (modifies given pointer), returns self
[[nodiscard]] static Result resize(List* self, size_t size)
{
	Array* oldArray = self->_array;
	size_t oldCount = self->Count;
	
	try (List_Constructor1(self, size))
	end 

	self->Count = oldCount;

	for (size_t i = 0; i < oldCount; i++)
	{
		void* oldItem; 
		try (Array_At(oldArray, i))
		out (oldItem)

		try (Array_Set(self->_array, i, oldItem))
		end
	}

	delete(Array, oldArray);

	return (Result) {OK, self};
}

// Constructs a List of size and returns the self pointer
[[nodiscard]] Result List_Constructor1(List* self, size_t size)
{
	if (size <= 0)
	{
		return (Result) {InvalidArgument};
	}

	Array* array;
	try (new2(Array, sizeof(void*), size))
	out (array)

	*self = (List)
	{
		.Count = 0,
		._array = array
	};

	return (Result) {OK, self};
}
// Frees the resources held, and returns reference to self
List* List_Destructor(List* self)
{
	delete(Array, self->_array);
	return self;
}
// Returns the item at given index; or NULL if out of bounds
Result List_At(List* self, size_t index)
{
	if (index < 0 || self->Count <= index)
	{
		return (Result) {OutOfBounds};
	}

	void* location;
	try (Array_At(self->_array, index))
	out (location);

	return (Result) {OK, *(void**)location};
}
// Sets the Item at given index
Result List_Set(List* self, size_t index, const void* item)
{
	if (index < 0 || self->Count <= index)
	{
		return (Result) {OutOfBounds};
	}

	return Array_Set(self->_array, index, &item);
}
// Returns the first Item
Result List_Front(List* self)
{
	return List_At(self, 0);
}
// Returns the last Item
Result List_Back(List* self)
{
	return List_At(self, self->Count - 1);
}
// Inserts the Item at given index and returns it
Result List_Insert(List* self, size_t index, const void* item)
{
	if (index < 0 || self->Count < index)
	{
		return (Result) {OutOfBounds};
	}

	if (self->Count == self->_array->Count)
	{
		try (resize(self, self->_array->Count * 2))
		end
	}

	for (size_t i = self->Count; i > index; i--)
	{
		void* previous;
		try (List_At(self, i - 1))
		out (previous)

		try (Array_Set(self->_array, i, &previous))
		end
	}

	try (Array_Set(self->_array, index, &item))
	end

	self->Count++;

	return List_At(self, index);
}
// Inserts the Item at front and returns it, or NULL if any error
Result List_PushFront(List* self, const void* item)
{
	return List_Insert(self, 0, item);
}
// Inserts the Item at end and returns it, or NULL if any error
Result List_PushBack(List* self, const void* item)
{
	return List_Insert(self, self->Count, item);
}
// Removes the Item at given index and returns removed. Returns NULL if out of bounds, or any error
Result List_Remove(List* self, size_t index)
{
	try (List_At(self, index))
	end

	void* removed;
	try (List_At(self, index))
	out (removed)

	for (size_t i = index; i < self->Count - 1; i++)
	{
		void* next;
		try (List_At(self, i + 1))
		out (next)
		
		try (Array_Set(self->_array, i, &next))
		end
	}

	self->Count--;
	
	if (self->Count == self->_array->Count / 4)
	{
		try (resize(self, self->_array->Count / 2))
		end
	}

	return (Result) {OK, removed};
}
// Removes the Item at front and returns removed, or NULL if any error
Result List_RemoveFront(List* self)
{
	return List_Remove(self, 0);
}
// Removes the Item at end and returns removed, or NULL if any error
Result List_RemoveBack(List* self)
{
	return List_Remove(self, self->Count - 1);
}