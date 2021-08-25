//--------------------------------------------------
// Atta Memory Tests
// stackAllocator.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/memorySystem/allocators/stackAllocator.h>

using namespace atta;
namespace
{
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

	TEST(Memory_StackAllocator, Allocate)
	{
		StackAllocator stack(1024);
		int *i0;

		i0 = stack.alloc<int>();
		*i0 = 10;
		EXPECT_EQ(*i0, 10);
	}

	TEST(Memory_StackAllocator, Rollback)
	{
		StackAllocator stack(1024);

		// Allocate 2 bytes
		stack.alloc<uint16_t>();

		StackAllocator::Marker marker = stack.getMarker();
		// Check if marker is where it should be
		EXPECT_EQ(marker, sizeof(uint16_t));

		TestObject* obj0 = stack.alloc<TestObject>();
		stack.rollback(marker);
		TestObject* obj1 = stack.alloc<TestObject>();

		// Check if returned same pointer after rollback
		EXPECT_EQ(obj0, obj1);
	}

	TEST(Memory_StackAllocator, AllocateFail)
	{
		StackAllocator stack(1024);

		// Should not be possible to alloc more than the stack supports
		TestObject* obj = stack.alloc<TestObject>(10);
		EXPECT_EQ(obj, nullptr);
	}

	TEST(Memory_StackAllocator, Clear)
	{
		StackAllocator stack(1024);

		stack.alloc<int>(20);
		EXPECT_EQ(stack.getMarker(), 20*sizeof(int));

		stack.clear();
		EXPECT_EQ(stack.getMarker(), 0);
	}

	TEST(Memory_StackAllocator, Free)
	{
		StackAllocator stack(1024);
		int *i0, *i1, *i2;
		char *c0;

		// Allocate
		i0 = stack.alloc<int>();
		c0 = stack.alloc<char>();
		i1 = stack.alloc<int>();

		// Set values
		*i0 = 10;
		*c0 = 'a';
		*i1 = 20;

		// Test values
		EXPECT_EQ(*i0, 10);
		EXPECT_EQ(*c0, 'a');
		EXPECT_EQ(*i1, 20);

		// Free
		stack.free<int>(i1);
		stack.free<int>(i0);// Will not be freed
		stack.free<char>(c0);

		// Allocate again
		c0 = stack.alloc<char>();
		i2 = stack.alloc<int>();

		// The values should not have changed
		EXPECT_EQ(*c0, 'a');
		EXPECT_EQ(*i2, 20);
	}

	TEST(Memory_StackAllocator, Alignment)
	{
		GTEST_SKIP();

		StackAllocator stack(1024);
		int *i0, *i1;
		char *c0, *c1;
		uint16_t *u0;

		// iiii------------
		i0 = stack.alloc<int>();
		EXPECT_EQ(stack.getMarker(), sizeof(int));

		// iiiic-----------
		c0 = stack.alloc<char>();
		EXPECT_EQ(stack.getMarker(), sizeof(int)+sizeof(char));

		// iiiic-uu--------
		u0 = stack.alloc<uint16_t>();
		EXPECT_EQ(stack.getMarker(), 4*sizeof(uint16_t));

		// iiiic-uuc-------
		c1 = stack.alloc<char>();
		EXPECT_EQ(stack.getMarker(), 4*sizeof(uint16_t)+sizeof(char));

		// iiiic-uuc---iiii
		i1 = stack.alloc<int>();
		EXPECT_EQ(stack.getMarker(), 4*sizeof(int));

		delete i1;
		delete c1;
		delete u0;
		delete c0;
		delete i0;
	}
}
