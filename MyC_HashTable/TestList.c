#include <assert.h>

#include "List.h"

static void List_Initialize_ShouldHaveZeroItems()
{
	// Arrange
	List list;

	// Act
	$List.Constructor(&list, 42);
	
	// Assert
	assert(list._size == 42);
	assert(list.Count == 0);
	
	// Annihilate
	$List.Destructor(&list);
}
static void List_At_ShouldReturnItemAtLocation()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	
	// Assert
	assert(list._size == 2);
	assert(list.Count == 2);
	assert($List.At(&list, 0) == list._items[0]);
	assert($List.At(&list, 1) == list._items[1]);

	// Annihilate
	$List.Destructor(&list);
}
static void List_Front_ShouldReturnItemAtFront()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	
	// Assert
	assert(list._size == 2);
	assert(list.Count == 2);
	assert($List.Front(&list) == list._items[0]);

	// Annihilate
	$List.Destructor(&list);
}
static void List_Back_ShouldReturnItemAtEnd()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	
	// Assert
	assert(list._size == 2);
	assert(list.Count == 2);
	assert($List.Back(&list) == list._items[1]);

	// Annihilate
	$List.Destructor(&list);
}
static void List_PushBack_ShouldAddItemToEnd()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	
	// Assert
	assert(list._size == 2);
	assert(list.Count == 2);
	assert($List.At(&list, 0) == &item0);
	assert($List.At(&list, 1) == &item1);

	// Annihilate
	$List.Destructor(&list);
}
static void List_PushFront_ShouldAddItemToFront()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1;

	// Act
	assert($List.PushFront(&list, &item0));
	assert($List.PushFront(&list, &item1));
	
	// Assert
	assert(list._size == 2);
	assert(list.Count == 2);
	assert($List.At(&list, 0) == &item1);
	assert($List.At(&list, 1) == &item0);

	// Annihilate
	$List.Destructor(&list);
}
static void List_RemoveFront_ShouldRemoveItemAtFront()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	assert($List.RemoveFront(&list));
	
	// Assert
	assert(list._size == 2);
	assert(list.Count == 1);
	assert($List.At(&list, 0) == &item1);

	// Annihilate
	$List.Destructor(&list);
}
static void List_RemoveBack_ShouldRemoveItemAtBack()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	assert($List.RemoveBack(&list));
	
	// Assert
	assert(list._size == 2);
	assert(list.Count == 1);
	assert($List.At(&list, 0) == &item0);

	// Annihilate
	$List.Destructor(&list);
}
static void List_Remove_ShouldRemoveItemAtGivenIndex()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1, item2;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	assert($List.PushBack(&list, &item2));
	assert($List.Remove(&list, 1));
	
	// Assert
	assert(list._size == 4);
	assert(list.Count == 2);
	assert($List.At(&list, 0) == &item0);
	assert($List.At(&list, 1) == &item2);

	// Annihilate
	$List.Destructor(&list);
}
static void List_Insert_ShouldInsertItemAtGivenIndex()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item0, item1, item2, itemX;

	// Act
	assert($List.PushBack(&list, &item0));
	assert($List.PushBack(&list, &item1));
	assert($List.PushBack(&list, &item2));
	assert($List.Insert(&list, 1, &itemX));
	
	// Assert
	assert(list._size == 4);
	assert(list.Count == 4);
	assert($List.At(&list, 0) == &item0);
	assert($List.At(&list, 1) == &itemX);
	assert($List.At(&list, 2) == &item1);
	assert($List.At(&list, 3) == &item2);

	// Annihilate
	$List.Destructor(&list);
}

static void List_CapacityReached_ShouldDoubleSize()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item;

	// Act & Assert
	assert(list._size == 1);
	assert(list.Count == 0);
	
	assert($List.PushBack(&list, &item));
	assert($List.PushBack(&list, &item));
	assert(list._size == 2);
	assert(list.Count == 2);
	
	assert($List.PushBack(&list, &item));
	assert(list._size == 4);
	assert(list.Count == 3);
	
	assert($List.PushBack(&list, &item));
	assert($List.PushBack(&list, &item));
	assert(list._size == 8);
	assert(list.Count == 5);

	// Annihilate
	$List.Destructor(&list);
}
static void List_WhenHasLessItems_ShouldReduceSize()
{
	// Arrange
	List list;
	assert($List.Constructor(&list, 1));
	
	int item;

	// Act & Assert
	assert(list._size == 1);
	assert(list.Count == 0);
	
	assert($List.PushBack(&list, &item));
	assert($List.PushBack(&list, &item));
	assert($List.PushBack(&list, &item));
	assert($List.PushBack(&list, &item));
	assert($List.PushBack(&list, &item));
	assert(list._size == 8);
	assert(list.Count == 5);

	assert($List.RemoveBack(&list));
	assert($List.RemoveBack(&list));
	assert(list._size == 8);

	assert($List.RemoveBack(&list));
	assert(list._size == 4);
	assert(list.Count == 2);

	assert($List.RemoveBack(&list));
	assert(list._size == 2);
	assert(list.Count == 1);

	assert($List.RemoveBack(&list));
	assert(list._size == 1);
	assert(list.Count == 0);

	// Can't do
	assert(!$List.RemoveBack(&list));
	assert(list._size == 1);
	assert(list.Count == 0);

	// Annihilate
	$List.Destructor(&list);
}

void RunAll()
{
	List_Initialize_ShouldHaveZeroItems();
	List_At_ShouldReturnItemAtLocation();
	List_Front_ShouldReturnItemAtFront();
	List_Back_ShouldReturnItemAtEnd();
	List_PushFront_ShouldAddItemToFront();
	List_PushBack_ShouldAddItemToEnd();
	List_RemoveFront_ShouldRemoveItemAtFront();
	List_RemoveBack_ShouldRemoveItemAtBack();
	List_Remove_ShouldRemoveItemAtGivenIndex();
	List_Insert_ShouldInsertItemAtGivenIndex();

	List_CapacityReached_ShouldDoubleSize();
	List_WhenHasLessItems_ShouldReduceSize();
}

#include "TestList.c.gen"