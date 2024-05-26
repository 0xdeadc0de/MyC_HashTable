#include <assert.h>
#include <string.h>

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
static void Array_AsStringType_WorksAsExpected()
{
	// Arrange
	Array array;
	const char string[] = "hello testing string";
	const int length = 20+1;
	$Array.Constructor(&array, sizeof(char), length);

	// Act
	for (int i = 0; i < length; i++)
	{
		$Array.Set(&array, i, &string[i]);
	}

	// Assert
	for (int i = 0; i < length; i++)
	{
		void* location = $Array.At(&array, i);
		char* charLocation = location;
		int value = *charLocation;
		assert(string[i] == value);
	}
	assert(!strcmp(string, array.Items));
	
	// Annihilate
	$Array.Destructor(&array);
}
static void Array_AsCustomType_WorksAsExpected()
{
	// Arrange
	typedef struct
	{
		int number;
		long array[10];
	}
	CustomType;

	Array array;
	const length = 3;
	CustomType customTypes[3] =
	{
		(CustomType)
		{
			.number = 0,
			.array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
		},
		(CustomType)
		{
			.number = 1,
			.array = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19}
		},
		(CustomType)
		{
			.number = 2,
			.array = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29}
		},
	};

	$Array.Constructor(&array, sizeof(CustomType), length);

	// Act
	for (int i = 0; i < length; i++)
	{
		$Array.Set(&array, i, &customTypes[i]);
	}

	// Assert
	for (int i = 0; i < length; i++)
	{
		void* location = $Array.At(&array, i);
		CustomType* customTypeLocation = location;
		CustomType value = *customTypeLocation;
		assert(customTypes[i].number == value.number);
		for (int j = 0; j < 10; j++)
		{
			assert(customTypes[i].array[j] == value.array[j]);
		}
	}
	
	// Annihilate
	$Array.Destructor(&array);
}

static void RunAll()
{
	Array_SetsItem_AbleToRetrieveByAtMethod();
	Array_SetsAllItems_AbleToRetrieveAllItems();
	Array_AsStringType_WorksAsExpected();
	Array_AsCustomType_WorksAsExpected();
}

#include "TestArray.c.gen"