#undef MyC_Test

#include <assert.h>

#include "List.h"

static void List_Initialize_ShouldHaveZeroItems()
{
	// Arrange
	List list;

	// Act
	List_Constructor1(&list, 42);
	
	// Assert
	assert(list._array->Count == 42);
	assert(list.Count == 0);
	
	// Annihilate
	List_Destructor(&list);
}
static void List_At_ShouldReturnItemAtLocation()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	assert(List_PushBack(&list, itemLocation));
	
	// Assert
	assert(list._array->Count == 1);
	assert(list.Count == 1);
	assert(List_At(&list, 0) == *(void**)Array_At(list._array, 0));

	void* location = List_At(&list, 0);
	int* intLocation = location;
	int value = *intLocation;
	assert(42 == value);

	// Annihilate
	List_Destructor(&list);
}
static void List_Set_ShouldSetItemAtLocation()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	assert(List_PushBack(&list, itemLocation));
	int newItem = 38;
	int* newLocation = &newItem;
	assert(List_Set(&list, 0, newLocation));
	
	// Assert
	assert(list._array->Count == 1);
	assert(list.Count == 1);
	assert(List_At(&list, 0) == *(void**)Array_At(list._array, 0));

	void* location = List_At(&list, 0);
	assert(location == newLocation);

	// Annihilate
	List_Destructor(&list);
}

static void List_CapacityReached_ShouldDoubleSize()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	assert(List_PushBack(&list, &item));
	assert(List_PushBack(&list, &item));
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	
	assert(List_PushBack(&list, &item));
	assert(list._array->Count == 4);
	assert(list.Count == 3);
	
	assert(List_PushBack(&list, &item));
	assert(List_PushBack(&list, &item));
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	// Annihilate
	List_Destructor(&list);
}
static void List_WhenHasLessItems_ShouldReduceSize()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	assert(List_PushBack(&list, &item));
	assert(List_PushBack(&list, &item));
	assert(List_PushBack(&list, &item));
	assert(List_PushBack(&list, &item));
	assert(List_PushBack(&list, &item));
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	assert(List_RemoveBack(&list));
	assert(List_RemoveBack(&list));
	assert(list._array->Count == 8);

	assert(List_RemoveBack(&list));
	assert(list._array->Count == 4);
	assert(list.Count == 2);

	assert(List_RemoveBack(&list));
	assert(list._array->Count == 2);
	assert(list.Count == 1);

	assert(List_RemoveBack(&list));
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Can't do
	assert(!List_RemoveBack(&list));
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Annihilate
	List_Destructor(&list);
}

static void List_Front_ShouldReturnItemAtFront()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	assert(List_PushBack(&list, &item0));
	assert(List_PushBack(&list, &item1));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	assert(List_Front(&list) == *(void**)Array_At(list._array, 0));
	assert(List_Front(&list) == List_At(&list, 0));

	
	void* a = List_Front(&list), *b = &item0;
	assert(a == b);

	assert(List_Front(&list) == &item0);

	// Annihilate
	List_Destructor(&list);
}
static void List_Back_ShouldReturnItemAtEnd()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	assert(List_PushBack(&list, &item0));
	assert(List_PushBack(&list, &item1));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	assert(List_Back(&list) == *(void**)Array_At(list._array, 1));
	assert(List_Back(&list) == List_At(&list, 1));
	assert(List_Back(&list) == &item1);

	// Annihilate
	List_Destructor(&list);
}
static void List_PushBack_ShouldAddItemToEnd()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	assert(List_PushBack(&list, &item0));
	assert(List_PushBack(&list, &item1));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	assert(List_At(&list, 0) == &item0);
	assert(List_At(&list, 1) == &item1);

	// Annihilate
	List_Destructor(&list);
}
static void List_PushFront_ShouldAddItemToFront()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	assert(List_PushFront(&list, &item0));
	assert(List_PushFront(&list, &item1));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	assert(List_At(&list, 0) == &item1);
	assert(List_At(&list, 1) == &item0);

	// Annihilate
	List_Destructor(&list);
}
static void List_RemoveFront_ShouldRemoveItemAtFront()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	assert(List_PushBack(&list, &item0));
	assert(List_PushBack(&list, &item1));
	assert(List_RemoveFront(&list));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 1);
	assert(List_At(&list, 0) == &item1);

	// Annihilate
	List_Destructor(&list);
}
static void List_RemoveBack_ShouldRemoveItemAtBack()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1;

	// Act
	assert(List_PushBack(&list, &item0));
	assert(List_PushBack(&list, &item1));
	assert(List_RemoveBack(&list));
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 1);
	assert(List_At(&list, 0) == &item0);

	// Annihilate
	List_Destructor(&list);
}
static void List_Remove_ShouldRemoveItemAtGivenIndex()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1, item2;

	// Act
	assert(List_PushBack(&list, &item0));
	assert(List_PushBack(&list, &item1));
	assert(List_PushBack(&list, &item2));
	assert(List_Remove(&list, 1));
	
	// Assert
	assert(list._array->Count == 4);
	assert(list.Count == 2);
	assert(List_At(&list, 0) == &item0);
	assert(List_At(&list, 1) == &item2);

	// Annihilate
	List_Destructor(&list);
}
static void List_Insert_ShouldInsertItemAtGivenIndex()
{
	// Arrange
	List list;
	assert(List_Constructor1(&list, 1));
	
	int item0, item1, item2, itemX;

	// Act
	assert(List_PushBack(&list, &item0));
	assert(List_PushBack(&list, &item1));
	assert(List_PushBack(&list, &item2));
	assert(List_Insert(&list, 1, &itemX));
	
	// Assert
	assert(list._array->Count == 4);
	assert(list.Count == 4);
	assert(List_At(&list, 0) == &item0);
	assert(List_At(&list, 1) == &itemX);
	assert(List_At(&list, 2) == &item1);
	assert(List_At(&list, 3) == &item2);

	// Annihilate
	List_Destructor(&list);
}

#include "TestList.c.gen"