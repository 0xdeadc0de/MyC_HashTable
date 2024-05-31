#undef MyC_Test

#include <assert.h>

#include "List.h"

static Result(ref) List_Initialize_ShouldHaveZeroItems()
{
	// Arrange
	List list;

	// Act
	ret (ref);
	run (List_Constructor1(&list, 42));;
	
	// Assert
	assert(list._array->Count == 42);
	assert(list.Count == 0);
	
	// Annihilate
	List_Destructor(&list);
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

	void *a, *b;
	try_old (List_At(&list, 0))
	out_old (a)
	try_old (Array_At(list._array, 0))
	out_old (b)
	assert(a == *(void**)b);

	void* location;
	try_old (List_At(&list, 0))
	out_old (location)

	int* intLocation = location;
	int value = *intLocation;
	assert(42 == value);

	// Annihilate
	List_Destructor(&list);
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
	
	void *a, *b;
	try_old (List_At(&list, 0))
	out_old (a)
	try_old (Array_At(list._array, 0))
	out_old (b)
	assert(a == *(void**)b);

	void* location;
	try_old (List_At(&list, 0))
	out_old (location)

	assert(location == newLocation);

	// Annihilate
	List_Destructor(&list);
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
	assert(List_RemoveBack(&list).code == out_oldOfBounds);
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Annihilate
	List_Destructor(&list);
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
	
	void* front;
	try_old (List_Front(&list))
	out_old (front)

	void* zeroth;
	try_old (Array_At(list._array, 0))
	out_old (zeroth)

	void* listZero;
	try_old (List_At(&list, 0))
	out_old (listZero)

	assert(front == *(void**)zeroth);
	assert(front == listZero);

	void* a = front, *b = &item0;
	assert(a == b);

	assert(front == &item0);

	// Annihilate
	List_Destructor(&list);
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
	void* listBack;
	try_old (List_Back(&list))
	out_old (listBack)

	void* first;
	try_old (Array_At(list._array, 1))
	out_old (first)

	void* listFirst;
	try_old (List_At(&list, 1))
	out_old (listFirst)

	assert(list._array->Count == 2);
	assert(list.Count == 2);
	assert(listBack == *(void**)first);
	assert(listBack == listFirst);
	assert(listBack == &item1);

	// Annihilate
	List_Destructor(&list);
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

	void* zeroth;
	try_old (List_At(&list, 0))
	out_old (zeroth)

	void* first;
	try_old (List_At(&list, 1))
	out_old (first)

	assert(zeroth == &item0);
	assert(first == &item1);

	// Annihilate
	List_Destructor(&list);
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

	void* zeroth;
	try_old (List_At(&list, 0))
	out_old (zeroth)

	void* first;
	try_old (List_At(&list, 1))
	out_old (first)

	assert(zeroth == &item1);
	assert(first == &item0);
	
	// Annihilate
	List_Destructor(&list);
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


	void* zeroth;
	try_old (List_At(&list, 0))
	out_old (zeroth)

	assert(zeroth == &item1);

	// Annihilate
	List_Destructor(&list);
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
	
	void* zeroth;
	try_old (List_At(&list, 0))
	out_old (zeroth)

	assert(zeroth == &item0);

	// Annihilate
	List_Destructor(&list);
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
	
	void* zeroth;
	try_old (List_At(&list, 0))
	out_old (zeroth)

	void* first;
	try_old (List_At(&list, 1))
	out_old (first)

	assert(zeroth == &item0);
	assert(first == &item2);
	
	// Annihilate
	List_Destructor(&list);
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

	
	void* a;
	try_old (List_At(&list, 0))
	out_old (a)
	assert(a == &item0);

	try_old (List_At(&list, 1))
	out_old (a)
	assert(a == &itemX);

	try_old (List_At(&list, 2))
	out_old (a)
	assert(a == &item1);

	try_old (List_At(&list, 3))
	out_old (a)
	assert(a == &item2);

	// Annihilate
	List_Destructor(&list);
}

#include "TestList.c.gen"