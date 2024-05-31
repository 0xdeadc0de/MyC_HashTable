#undef MyC_Test

#include <assert.h>

#include "List.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static Result(ref) List_Initialize_ShouldHaveZeroItems()
{
	// Arrange
	List list;

	// Act
	ret (ref);
	run (List_Constructor1(&list, 42));
	
	// Assert
	assert(list._array->Count == 42);
	assert(list.Count == 0);
	
	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_At_ShouldReturnItemAtLocation()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	run (List_PushBack(&list, itemLocation));
	
	// Assert
	assert(list._array->Count == 1);
	assert(list.Count == 1);

	try (ref, a, List_At(&list, 0));
	try (ref, b, Array_At(list._array, 0));
	assert(a == *(void**)b);

	try (ref, location, List_At(&list, 0));

	int* intLocation = location;
	int value = *intLocation;
	assert(42 == value);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_Set_ShouldSetItemAtLocation()
{
	ret (ref);
	// Arrange
	List list;
	run (List_Constructor1(&list, 1));
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	run (List_PushBack(&list, itemLocation));
	int newItem = 38;
	int* newLocation = &newItem;
	run (List_Set(&list, 0, newLocation));
	
	// Assert
	assert(list._array->Count == 1);
	assert(list.Count == 1);
	
	try (ref, a, List_At(&list, 0));
	try (ref, b, Array_At(list._array, 0));
	assert(a == *(void**)b);

	try (ref, location, List_At(&list, 0));

	assert(location == newLocation);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}

static Result(ref) List_CapacityReached_ShouldDoubleSize()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	run (List_PushBack(&list, &item));
	run (List_PushBack(&list, &item));
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	
	run (List_PushBack(&list, &item));
	assert(list._array->Count == 4);
	assert(list.Count == 3);
	
	run (List_PushBack(&list, &item));
	run (List_PushBack(&list, &item));
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_WhenHasLessItems_ShouldReduceSize()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	run (List_PushBack(&list, &item));
	run (List_PushBack(&list, &item));
	run (List_PushBack(&list, &item));
	run (List_PushBack(&list, &item));
	run (List_PushBack(&list, &item));
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	run (List_RemoveBack(&list));
	run (List_RemoveBack(&list));
	assert(list._array->Count == 8);

	run (List_RemoveBack(&list));
	assert(list._array->Count == 4);
	assert(list.Count == 2);

	run (List_RemoveBack(&list));
	assert(list._array->Count == 2);
	assert(list.Count == 1);

	run (List_RemoveBack(&list));
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Can't do
	assert(List_RemoveBack(&list).code == OutOfBounds);
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}

static Result(ref) List_Front_ShouldReturnItemAtFront()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	run (List_PushBack(&list, &item0));
	run (List_PushBack(&list, &item1));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	
	try (ref, front, List_Front(&list));

	try (ref, zeroth, Array_At(list._array, 0));

	try (ref, listZero, List_At(&list, 0));

	assert(front == *(void**)zeroth);
	assert(front == listZero);

	void* a = front, *b = &item0;
	assert(a == b);

	assert(front == &item0);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_Back_ShouldReturnItemAtEnd()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	run (List_PushBack(&list, &item0));
	run (List_PushBack(&list, &item1));
	
	// Assert
	try (ref, listBack, List_Back(&list));

	try (ref, first, Array_At(list._array, 1));

	try (ref, listFirst, List_At(&list, 1));

	assert(list._array->Count == 2);
	assert(list.Count == 2);
	assert(listBack == *(void**)first);
	assert(listBack == listFirst);
	assert(listBack == &item1);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_PushBack_ShouldAddItemToEnd()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	run (List_PushBack(&list, &item0));
	run (List_PushBack(&list, &item1));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);

	try (ref, zeroth, List_At(&list, 0));

	try (ref, first, List_At(&list, 1));

	assert(zeroth == &item0);
	assert(first == &item1);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_PushFront_ShouldAddItemToFront()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	run (List_PushFront(&list, &item0));
	run (List_PushFront(&list, &item1));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);

	try (ref, zeroth, List_At(&list, 0));

	try (ref, first, List_At(&list, 1));

	assert(zeroth == &item1);
	assert(first == &item0);
	
	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_RemoveFront_ShouldRemoveItemAtFront()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	run (List_PushBack(&list, &item0));
	run (List_PushBack(&list, &item1));
	run (List_RemoveFront(&list));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 1);


	try (ref, zeroth, List_At(&list, 0));

	assert(zeroth == &item1);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_RemoveBack_ShouldRemoveItemAtBack()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	run (List_PushBack(&list, &item0));
	run (List_PushBack(&list, &item1));
	run (List_RemoveBack(&list));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 1);
	
	try (ref, zeroth, List_At(&list, 0));

	assert(zeroth == &item0);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_Remove_ShouldRemoveItemAtGivenIndex()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1, item2;

	// Act
	run (List_PushBack(&list, &item0));
	run (List_PushBack(&list, &item1));
	run (List_PushBack(&list, &item2));
	run (List_Remove(&list, 1));
	
	// Assert
	assert(list._array->Count == 4);
	assert(list.Count == 2);
	
	try (ref, zeroth, List_At(&list, 0));

	try (ref, first, List_At(&list, 1));

	assert(zeroth == &item0);
	assert(first == &item2);
	
	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}
static Result(ref) List_Insert_ShouldInsertItemAtGivenIndex()
{
	// Arrange
	List list;
	ret (ref);
	run (List_Constructor1(&list, 1));
	
	int item0, item1, item2, itemX;

	// Act
	run (List_PushBack(&list, &item0));
	run (List_PushBack(&list, &item1));
	run (List_PushBack(&list, &item2));
	run (List_Insert(&list, 1, &itemX));
	
	// Assert
	assert(list._array->Count == 4);
	assert(list.Count == 4);

	
	try (ref, a, List_At(&list, 0));
	assert(a == &item0);

	try (ref, a1, List_At(&list, 1));
	assert(a1 == &itemX);

	try (ref, a2, List_At(&list, 2));
	assert(a2 == &item1);

	try (ref, a3, List_At(&list, 3));
	assert(a3 == &item2);

	// Annihilate
	List_Destructor(&list);
	
	return OK(NULL);
}

#pragma GCC diagnostic pop

#include "TestList.c.gen"