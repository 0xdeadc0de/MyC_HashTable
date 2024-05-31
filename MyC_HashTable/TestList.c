#undef MyC_Test

#include <assert.h>

#include "List.h"

static Result List_Initialize_ShouldHaveZeroItems()
{
	// Arrange
	List list;

	// Act
	try_old (List_Constructor1(&list, 42))
	end_old;
	
	// Assert
	assert(list._array->Count == 42);
	assert(list.Count == 0);
	
	// Annihilate
	List_Destructor(&list);
}
static Result List_At_ShouldReturnItemAtLocation()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	try_old (List_PushBack(&list, itemLocation))
	end_old
	
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
static Result List_Set_ShouldSetItemAtLocation()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	try_old (List_PushBack(&list, itemLocation))
	end_old
	int newItem = 38;
	int* newLocation = &newItem;
	try_old (List_Set(&list, 0, newLocation))
	end_old
	
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

static Result List_CapacityReached_ShouldDoubleSize()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	try_old (List_PushBack(&list, &item))
	end_old
	try_old (List_PushBack(&list, &item))
	end_old
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	
	try_old (List_PushBack(&list, &item))
	end_old
	assert(list._array->Count == 4);
	assert(list.Count == 3);
	
	try_old (List_PushBack(&list, &item))
	end_old
	try_old (List_PushBack(&list, &item))
	end_old
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	// Annihilate
	List_Destructor(&list);
}
static Result List_WhenHasLessItems_ShouldReduceSize()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	try_old (List_PushBack(&list, &item))
	end_old
	try_old (List_PushBack(&list, &item))
	end_old
	try_old (List_PushBack(&list, &item))
	end_old
	try_old (List_PushBack(&list, &item))
	end_old
	try_old (List_PushBack(&list, &item))
	end_old
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	try_old (List_RemoveBack(&list))
	end_old
	try_old (List_RemoveBack(&list))
	end_old
	assert(list._array->Count == 8);

	try_old (List_RemoveBack(&list))
	end_old
	assert(list._array->Count == 4);
	assert(list.Count == 2);

	try_old (List_RemoveBack(&list))
	end_old
	assert(list._array->Count == 2);
	assert(list.Count == 1);

	try_old (List_RemoveBack(&list))
	end_old
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Can't do
	assert(List_RemoveBack(&list).code == out_oldOfBounds);
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Annihilate
	List_Destructor(&list);
}

static Result List_Front_ShouldReturnItemAtFront()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1;

	// Act
	try_old (List_PushBack(&list, &item0))
	end_old
	try_old (List_PushBack(&list, &item1))
	end_old
	
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
static Result List_Back_ShouldReturnItemAtEnd()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1;

	// Act
	try_old (List_PushBack(&list, &item0))
	end_old
	try_old (List_PushBack(&list, &item1))
	end_old
	
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
static Result List_PushBack_ShouldAddItemToEnd()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1;

	// Act
	try_old (List_PushBack(&list, &item0))
	end_old
	try_old (List_PushBack(&list, &item1))
	end_old
	
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
static Result List_PushFront_ShouldAddItemToFront()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1;

	// Act
	try_old (List_PushFront(&list, &item0))
	end_old
	try_old (List_PushFront(&list, &item1))
	end_old
	
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
static Result List_RemoveFront_ShouldRemoveItemAtFront()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1;

	// Act
	try_old (List_PushBack(&list, &item0))
	end_old
	try_old (List_PushBack(&list, &item1))
	end_old
	try_old (List_RemoveFront(&list))
	end_old
	
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
static Result List_RemoveBack_ShouldRemoveItemAtBack()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1;

	// Act
	try_old (List_PushBack(&list, &item0))
	end_old
	try_old (List_PushBack(&list, &item1))
	end_old
	try_old (List_RemoveBack(&list))
	end_old
	
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
static Result List_Remove_ShouldRemoveItemAtGivenIndex()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1, item2;

	// Act
	try_old (List_PushBack(&list, &item0))
	end_old
	try_old (List_PushBack(&list, &item1))
	end_old
	try_old (List_PushBack(&list, &item2))
	end_old
	try_old (List_Remove(&list, 1))
	end_old
	
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
static Result List_Insert_ShouldInsertItemAtGivenIndex()
{
	// Arrange
	List list;
	try_old (List_Constructor1(&list, 1))
	end_old
	
	int item0, item1, item2, itemX;

	// Act
	try_old (List_PushBack(&list, &item0))
	end_old
	try_old (List_PushBack(&list, &item1))
	end_old
	try_old (List_PushBack(&list, &item2))
	end_old
	try_old (List_Insert(&list, 1, &itemX))
	end_old
	
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