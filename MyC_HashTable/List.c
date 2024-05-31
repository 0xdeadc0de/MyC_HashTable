#include <stdlib.h>

#include "List.h"

// Resizes the List (modifies given pointer), returns self
[[nodiscard]] static Result(ref) resize(List* self, size_t size)
{
	Array* oldArray = self->_array;
	size_t oldCount = self->Count;
	
	ret (ref);
	run (List_Constructor1(self, size)); 

	self->Count = oldCount;

	for (size_t i = 0; i < oldCount; i++)
	{
		void* oldItem; 
		try_old (Array_At(oldArray, i))
		out_old (oldItem)

		run (Array_Set(self->_array, i, oldItem));
	}

	delete(Array, oldArray);

	return (Result(ref)) {OK, self};
}

// Constructs a List of size and returns the self pointer
[[nodiscard]] Result(ref) List_Constructor1(List* self, size_t size)
{
	if (size <= 0)
	{
		return (Result(ref)) {InvalidArgument};
	}

	Array* array;
	try_old (new2(Array, sizeof(void*), size))
	out_old (array)

	*self = (List)
	{
		.Count = 0,
		._array = array
	};

	return (Result(ref)) {OK, self};
}
// Frees the resources held, and returns reference to self
List* List_Destructor(List* self)
{
	delete(Array, self->_array);
	return self;
}
// Returns the item at given index; or NULL if out_old of bounds
Result(ref) List_At(List* self, size_t index)
{
	if (index < 0 || self->Count <= index)
	{
		return (Result(ref)) {out_oldOfBounds};
	}

	void* location;
	try_old (Array_At(self->_array, index))
	out_old (location);

	return (Result(ref)) {OK, *(void**)location};
}
// Sets the Item at given index
Result(ref) List_Set(List* self, size_t index, const void* item)
{
	if (index < 0 || self->Count <= index)
	{
		return (Result(ref)) {out_oldOfBounds};
	}

	return Array_Set(self->_array, index, &item);
}
// Returns the first Item
Result(ref) List_Front(List* self)
{
	return List_At(self, 0);
}
// Returns the last Item
Result(ref) List_Back(List* self)
{
	return List_At(self, self->Count - 1);
}
// Inserts the Item at given index and returns it
Result(ref) List_Insert(List* self, size_t index, const void* item)
{
	ret (ref);
	
	if (index < 0 || self->Count < index)
	{
		return (Result(ref)) {out_oldOfBounds};
	}

	if (self->Count == self->_array->Count)
	{
		run (resize(self, self->_array->Count * 2));
	}

	for (size_t i = self->Count; i > index; i--)
	{
		void* previous;
		try_old (List_At(self, i - 1))
		out_old (previous)

		run (Array_Set(self->_array, i, &previous));
	}

	run (Array_Set(self->_array, index, &item));

	self->Count++;

	return List_At(self, index);
}
// Inserts the Item at front and returns it, or NULL if any error
Result(ref) List_PushFront(List* self, const void* item)
{
	return List_Insert(self, 0, item);
}
// Inserts the Item at end and returns it, or NULL if any error
Result(ref) List_PushBack(List* self, const void* item)
{
	return List_Insert(self, self->Count, item);
}
// Removes the Item at given index and returns removed. Returns NULL if out_old of bounds, or any error
Result(ref) List_Remove(List* self, size_t index)
{
	ret (ref);
	run (List_At(self, index));

	void* removed;
	try_old (List_At(self, index))
	out_old (removed)

	for (size_t i = index; i < self->Count - 1; i++)
	{
		void* next;
		try_old (List_At(self, i + 1))
		out_old (next)
		
		run (Array_Set(self->_array, i, &next));
	}

	self->Count--;
	
	if (self->Count == self->_array->Count / 4)
	{
		run (resize(self, self->_array->Count / 2));
	}

	return (Result(ref)) {OK, removed};
}
// Removes the Item at front and returns removed, or NULL if any error
Result(ref) List_RemoveFront(List* self)
{
	return List_Remove(self, 0);
}
// Removes the Item at end and returns removed, or NULL if any error
Result(ref) List_RemoveBack(List* self)
{
	return List_Remove(self, self->Count - 1);
}