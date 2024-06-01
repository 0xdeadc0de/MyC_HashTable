#include <stdlib.h>

#include "List.h"

#include "List.head.c.gen"

// Resizes the List (modifies given pointer), returns self
static Result(ref) resize(List* self, size_t size)
{
	Array* oldArray = getArray(self);
	size_t oldCount = self->Count;
	
	setup(ref);
	run (List_Constructor1(self, size)); 

	self->Count = oldCount;

	for (size_t i = 0; i < oldCount; i++)
	{
		try (ref, oldItem, Array_At(oldArray, i));

		run (Array_Set(getArray(self), i, oldItem));
	}

	delete(Array, oldArray);

	return (Result(ref)) {OK, self};
}

// Constructs a List of size and returns the self pointer
Result(ref) List_Constructor1(List* self, size_t size)
{
	if (size <= 0)
	{
		return (Result(ref)) {InvalidArgument};
	}

	setup(ref);
	try (ref, array, new2(Array, sizeof(void*), size));

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
	delete(Array, getArray(self));
	return self;
}
// Returns the item at given index
Result(ref) List_At(List* self, size_t index)
{
	if (index < 0 || self->Count <= index)
	{
		return (Result(ref)) {OutOfBounds};
	}

	setup(ref);
	try (ref, location, Array_At(getArray(self), index));

	return (Result(ref)) {OK, *(void**)location};
}
// Sets the Item at given index
Result(ref) List_Set(List* self, size_t index, const void* item)
{
	if (index < 0 || self->Count <= index)
	{
		return (Result(ref)) {OutOfBounds};
	}

	return Array_Set(getArray(self), index, &item);
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
	setup(ref);
	
	if (index < 0 || self->Count < index)
	{
		return (Result(ref)) {OutOfBounds};
	}

	if (self->Count == getArray(self)->Count)
	{
		run (resize(self, getArray(self)->Count * 2));
	}

	for (size_t i = self->Count; i > index; i--)
	{
		try (ref, previous, List_At(self, i - 1));

		run (Array_Set(getArray(self), i, &previous));
	}

	run (Array_Set(getArray(self), index, &item));

	self->Count++;

	return List_At(self, index);
}
// Inserts the Item at front and returns it
Result(ref) List_PushFront(List* self, const void* item)
{
	return List_Insert(self, 0, item);
}
// Inserts the Item at end and returns it
Result(ref) List_PushBack(List* self, const void* item)
{
	return List_Insert(self, self->Count, item);
}
// Removes the Item at given index and returns removed
Result(ref) List_Remove(List* self, size_t index)
{
	setup(ref);
	run (List_At(self, index));

	try (ref, removed, List_At(self, index));

	for (size_t i = index; i < self->Count - 1; i++)
	{
		try (ref, next, List_At(self, i + 1));
		
		run (Array_Set(getArray(self), i, &next));
	}

	self->Count--;
	
	if (self->Count == getArray(self)->Count / 4)
	{
		run (resize(self, getArray(self)->Count / 2));
	}

	return (Result(ref)) {OK, removed};
}
// Removes the Item at front and returns removed
Result(ref) List_RemoveFront(List* self)
{
	return List_Remove(self, 0);
}
// Removes the Item at end and returns removed
Result(ref) List_RemoveBack(List* self)
{
	return List_Remove(self, self->Count - 1);
}