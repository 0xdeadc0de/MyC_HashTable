#undef MyC_Test

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Array.h"

static Result Array_SetsItem_AbleToRetrieveByAtMethod()
{
	// Arrange
	Array array;
	ret ();
	run (Array_Constructor2(&array, sizeof(int), 1));

	// Act
	int item = 42;
	int* itemLocation = &item;

	run (Array_Set(&array, 0, itemLocation));
	
	// Assert
	try (ref, location, Array_At(&array, 0));

	int* intLocation = location;
	int value = *intLocation;
	assert(item == value);
	
	// Annihilate
	Array_Destructor(&array);

	return ok;
}
static Result Array_SetsAllItems_AbleToRetrieveAllItems()
{
	// Arrange
	Array array;
	const int length = 42;
	ret ();
	run (Array_Constructor2(&array, sizeof(int), length));

	// Act
	for (int i = 0; i < length; i++)
	{
		run (Array_Set(&array, i, &i));
	}

	// Assert
	for (int i = 0; i < length; i++)
	{
		try (ref, location, Array_At(&array, i));

		int* intLocation = location;
		int value = *intLocation;
		assert(i == value);
	}
	
	// Annihilate
	Array_Destructor(&array);

	return ok;
}
static Result Array_AsStringType_WorksAsExpected()
{
	// Arrange
	Array array;
	const char string[] = "hello testing string";
	const int length = 20+1;
	ret ();
	run (Array_Constructor2(&array, sizeof(char), length));

	// Act
	for (int i = 0; i < length; i++)
	{
		run (Array_Set(&array, i, &string[i]));
	}

	// Assert
	for (int i = 0; i < length; i++)
	{
		try (ref, location, Array_At(&array, i));

		char* charLocation = location;
		int value = *charLocation;
		assert(string[i] == value);
	}
	assert(!strcmp(string, array.Items));
	
	// Annihilate
	Array_Destructor(&array);

	return ok;
}
static Result Array_AsCustomType_WorksAsExpected()
{
	// Arrange
	typedef struct
	{
		int number;
		long array[10];
	}
	CustomType;

	Array array;
	const int length = 3;
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

	ret ();
	run (Array_Constructor2(&array, sizeof(CustomType), length));

	// Act
	for (int i = 0; i < length; i++)
	{
		run (Array_Set(&array, i, &customTypes[i]));
	}

	// Assert
	for (int i = 0; i < length; i++)
	{
		try (ref, location, Array_At(&array, i));

		CustomType* customTypeLocation = location;
		CustomType value = *customTypeLocation;
		assert(customTypes[i].number == value.number);
		for (int j = 0; j < 10; j++)
		{
			assert(customTypes[i].array[j] == value.array[j]);
		}
	}
	
	// Annihilate
	Array_Destructor(&array);
	
	return ok;
}

static Result ArrayEquals_DifferentSizeOfItems_ReturnsFalse()
{
	// Arrange
	Array a1, a2;
	ret ();
	run (Array_Constructor2(&a1, 2, 2));
	run (Array_Constructor2(&a1, 1, 2));

	// Act & Assert
	assert(false == Array_Equals(&a1, &a2));
	
	return ok;
}
static Result ArrayEquals_DifferentLength_ReturnsFalse()
{
	// Arrange
	Array a1, a2;
	ret ();
	run (Array_Constructor2(&a1, 2, 1));
	run (Array_Constructor2(&a2, 2, 2));

	// Act & Assert
	assert(false == Array_Equals(&a1, &a2));
	
	return ok;
}
static Result ArrayEquals_SameLengthAndSizeOfButDifferentData_ReturnsFalse()
{
	// Arrange
	Array a1, a2;
	ret ();
	run (Array_Constructor2(&a1, 2, 2));
	run (Array_Constructor2(&a2, 2, 2));
	
	int x = 1, y = 2;
	run (Array_Set(&a1, 0, &x));
	run (Array_Set(&a2, 0, &y));

	// Act & Assert
	assert(false == Array_Equals(&a1, &a2));
	
	return ok;
}
static Result ArrayEquals_SameLengthAndSizeOfAndData_ReturnsTrue()
{
	// Arrange
	Array a1, a2;
	ret ();
	run (Array_Constructor2(&a1, 2, 2));
	run (Array_Constructor2(&a2, 2, 2));
	
	int x = 2, y = 2;
	run (Array_Set(&a1, 0, &x));
	run (Array_Set(&a2, 0, &y));

	// Act & Assert
	assert(true == Array_Equals(&a1, &a2));
	
	return ok;
}

static Result Array_Clone_CreatesExactCopy()
{
	// Arrange
	ret ();
	try (ref, r, new2(Array, sizeof(int), 5));
	Array* a1 = r;

	for (size_t i = 0; i < 5; i++)
	{
		run (Array_Set(a1, i, &i));
	}

	// Act
	try (ref, cloned, Array_Clone(a1));

	// Assert
	assert(Array_Equals(a1, cloned));
	
	return ok;
}

#include "TestArray.c.gen"