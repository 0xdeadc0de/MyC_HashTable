#undef MyC_Test

#include <assert.h>

#include "List.h"

static Result List_Initialize_ShouldHaveZeroItems()
{
	// Arrange
	List list;

	// Act
	try (List_Constructor1(&list, 42))
	end;
	
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
	try (List_Constructor1(&list, 1))
	end
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	try (List_PushBack(&list, itemLocation))
	end
	
	// Assert
	assert(list._array->Count == 1);
	assert(list.Count == 1);

	void *a, *b;
	try (List_At(&list, 0))
	set (a)
	try (Array_At(list._array, 0))
	set (b)
	assert(a == *(void**)b);

	void* location;
	try (List_At(&list, 0))
	set (location)

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
	try (List_Constructor1(&list, 1))
	end
	
	int item = 42;
	int* itemLocation = &item;
	// Act
	try (List_PushBack(&list, itemLocation))
	end
	int newItem = 38;
	int* newLocation = &newItem;
	try (List_Set(&list, 0, newLocation))
	end
	
	// Assert
	assert(list._array->Count == 1);
	assert(list.Count == 1);
	
	void *a, *b;
	try (List_At(&list, 0))
	set (a)
	try (Array_At(list._array, 0))
	set (b)
	assert(a == *(void**)b);

	void* location;
	try (List_At(&list, 0))
	set (location)

	assert(location == newLocation);

	// Annihilate
	List_Destructor(&list);
}

static Result List_CapacityReached_ShouldDoubleSize()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	try (List_PushBack(&list, &item))
	end
	try (List_PushBack(&list, &item))
	end
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	
	try (List_PushBack(&list, &item))
	end
	assert(list._array->Count == 4);
	assert(list.Count == 3);
	
	try (List_PushBack(&list, &item))
	end
	try (List_PushBack(&list, &item))
	end
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	// Annihilate
	List_Destructor(&list);
}
static Result List_WhenHasLessItems_ShouldReduceSize()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item;

	// Act & Assert
	assert(list._array->Count == 1);
	assert(list.Count == 0);
	
	try (List_PushBack(&list, &item))
	end
	try (List_PushBack(&list, &item))
	end
	try (List_PushBack(&list, &item))
	end
	try (List_PushBack(&list, &item))
	end
	try (List_PushBack(&list, &item))
	end
	assert(list._array->Count == 8);
	assert(list.Count == 5);

	try (List_RemoveBack(&list))
	end
	try (List_RemoveBack(&list))
	end
	assert(list._array->Count == 8);

	try (List_RemoveBack(&list))
	end
	assert(list._array->Count == 4);
	assert(list.Count == 2);

	try (List_RemoveBack(&list))
	end
	assert(list._array->Count == 2);
	assert(list.Count == 1);

	try (List_RemoveBack(&list))
	end
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Can't do
	assert(List_RemoveBack(&list).code == OutOfBounds);
	assert(list._array->Count == 1);
	assert(list.Count == 0);

	// Annihilate
	List_Destructor(&list);
}

static Result List_Front_ShouldReturnItemAtFront()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1;

	// Act
	try (List_PushBack(&list, &item0))
	end
	try (List_PushBack(&list, &item1))
	end
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);
	
	void* front;
	try (List_Front(&list))
	set (front)

	void* zeroth;
	try (Array_At(list._array, 0))
	set (zeroth)

	void* listZero;
	try (List_At(&list, 0))
	set (listZero)

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
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1;

	// Act
	try (List_PushBack(&list, &item0))
	end
	try (List_PushBack(&list, &item1))
	end
	
	// Assert
	void* listBack;
	try (List_Back(&list))
	set (listBack)

	void* first;
	try (Array_At(list._array, 1))
	set (first)

	void* listFirst;
	try (List_At(&list, 1))
	set (listFirst)

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
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1;

	// Act
	try (List_PushBack(&list, &item0))
	end
	try (List_PushBack(&list, &item1))
	end
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);

	void* zeroth;
	try (List_At(&list, 0))
	set (zeroth)

	void* first;
	try (List_At(&list, 1))
	set (first)

	assert(zeroth == &item0);
	assert(first == &item1);

	// Annihilate
	List_Destructor(&list);
}
static Result List_PushFront_ShouldAddItemToFront()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1;

	// Act
	try (List_PushFront(&list, &item0))
	end
	try (List_PushFront(&list, &item1))
	end
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 2);

	void* zeroth;
	try (List_At(&list, 0))
	set (zeroth)

	void* first;
	try (List_At(&list, 1))
	set (first)

	assert(zeroth == &item1);
	assert(first == &item0);
	
	// Annihilate
	List_Destructor(&list);
}
static Result List_RemoveFront_ShouldRemoveItemAtFront()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1;

	// Act
	try (List_PushBack(&list, &item0))
	end
	try (List_PushBack(&list, &item1))
	end
	try (List_RemoveFront(&list))
	end
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 1);


	void* zeroth;
	try (List_At(&list, 0))
	set (zeroth)

	assert(zeroth == &item1);

	// Annihilate
	List_Destructor(&list);
}
static Result List_RemoveBack_ShouldRemoveItemAtBack()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1;

	// Act
	try (List_PushBack(&list, &item0))
	end
	try (List_PushBack(&list, &item1))
	end
	try (List_RemoveBack(&list))
	end
	
	// Assert
	assert(list._array->Count == 2);
	assert(list.Count == 1);
	
	void* zeroth;
	try (List_At(&list, 0))
	set (zeroth)

	assert(zeroth == &item0);

	// Annihilate
	List_Destructor(&list);
}
static Result List_Remove_ShouldRemoveItemAtGivenIndex()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1, item2;

	// Act
	try (List_PushBack(&list, &item0))
	end
	try (List_PushBack(&list, &item1))
	end
	try (List_PushBack(&list, &item2))
	end
	try (List_Remove(&list, 1))
	end
	
	// Assert
	assert(list._array->Count == 4);
	assert(list.Count == 2);
	
	void* zeroth;
	try (List_At(&list, 0))
	set (zeroth)

	void* first;
	try (List_At(&list, 1))
	set (first)

	assert(zeroth == &item0);
	assert(first == &item2);
	
	// Annihilate
	List_Destructor(&list);
}
static Result List_Insert_ShouldInsertItemAtGivenIndex()
{
	// Arrange
	List list;
	try (List_Constructor1(&list, 1))
	end
	
	int item0, item1, item2, itemX;

	// Act
	try (List_PushBack(&list, &item0))
	end
	try (List_PushBack(&list, &item1))
	end
	try (List_PushBack(&list, &item2))
	end
	try (List_Insert(&list, 1, &itemX))
	end
	
	// Assert
	assert(list._array->Count == 4);
	assert(list.Count == 4);

	
	void* a;
	try (List_At(&list, 0))
	set (a)
	assert(a == &item0);

	try (List_At(&list, 1))
	set (a)
	assert(a == &itemX);

	try (List_At(&list, 2))
	set (a)
	assert(a == &item1);

	try (List_At(&list, 3))
	set (a)
	assert(a == &item2);

	// Annihilate
	List_Destructor(&list);
}

#include "TestList.c.gen"