//--------------------------------------------------
// Atta Memory Tests
// allocators.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/memory/allocators/stackAllocator.h>

using namespace atta;

class TestObject
{
public:
	TestObject(int startVal)
	{
		x[0] = y[0] = z[0] = startVal;
	}

	uint8_t x[100];
	uint8_t y[100];
	uint8_t z[100];
};

TEST(Memory, StackAllocator)
{
	StackAllocator stack(1024);
	int* test = stack.allocate<int>();
	*test = 10;
	// Test if value changed
	EXPECT_EQ(*test, 10);

	StackAllocator::Marker marker = stack.getMarker();
	// Check if marker is on where it should be
	EXPECT_EQ(marker, sizeof(int));

	TestObject* obj0 = stack.allocate<TestObject>();
	stack.rollback(marker);
	TestObject* obj1 = stack.allocate<TestObject>();

	// Check if returned same pointer after rollback
	EXPECT_EQ(obj0, obj1);
	// Check if value not changed
	EXPECT_EQ(*test, 10);

	// Should not be possible to allocate more than the stack supports
	TestObject* obj2 = stack.allocate<TestObject>(10);
	EXPECT_EQ(obj2, nullptr);

	// Clear stack, should return same pointer as the first allocation
	stack.clear();
	int* test1 = stack.allocate<int>();
	EXPECT_EQ(test, test1);

	// Test if only free objects at the top of the stack
	marker = stack.getMarker();
	stack.free<TestObject>(obj0);// Will not be freed
	EXPECT_EQ(stack.getMarker(), marker);

	stack.free<int>(test1);// Will be freed because it is the last pointer
	EXPECT_EQ(stack.getMarker(), 0);
}
