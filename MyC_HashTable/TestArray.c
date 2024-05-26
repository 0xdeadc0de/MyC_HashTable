#include <assert.h>

#include "Array.h"

static void Array_SetsItem_AbleToRetrieveByAtMethod()
{
	// Arrange
	Array array;
	$Array.Constructor(&array, sizeof(int), 1);

	// Act
	int item = 42;
	int* itemLocation = &item;
	$Array.Set(&array, 0, itemLocation);
	
	// Assert
	void* location = $Array.At(&array, 0);
	int* intLocation = location;
	int value = *intLocation;
	assert(item == value);
	
	// Annihilate
	$Array.Destructor(&array);
}
static void Array_SetsAllItems_AbleToRetrieveAllItems()
{
	// Arrange
	Array array;
	const length = 42;
	$Array.Constructor(&array, sizeof(int), length);

	// Act
	for (int i = 0; i < length; i++)
	{
		$Array.Set(&array, i, &i);
	}

	// Assert
	for (int i = 0; i < length; i++)
	{
		void* location = $Array.At(&array, i);
		int* intLocation = location;
		int value = *intLocation;
		assert(i == value);
	}
	
	// Annihilate
	$Array.Destructor(&array);
}

static void RunAll()
{
	Array_SetsItem_AbleToRetrieveByAtMethod();
	Array_SetsAllItems_AbleToRetrieveAllItems();
}

#include "TestArray.c.gen"