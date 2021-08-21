//--------------------------------------------------
// Atta Memory Tests
// speed.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/core/stringId.h>
#include <atta/memory/memoryManager.h>
#include <atta/memory/allocatedObject.h>
#include <atta/memory/allocators/stackAllocator.h>

using namespace atta;
namespace
{
	constexpr int NUM_IT = 2000;
	constexpr int NUM_OBJ = 5000;

	class TestMem : public AllocatedObject<TestMem, SID("Stack")>
	{
	public:
		TestMem() {}
		int x, y, z;
	};

	class TestMemMalloc
	{
	public:
		TestMemMalloc() {}
		int x, y, z;
	};

	class Memory_Speed : public ::testing::Test
	{
	public:
		void SetUp()
		{
			MemoryManager::registerAllocator(
				SID("Stack"), 
				std::static_pointer_cast<Allocator>(std::make_shared<StackAllocator>(sizeof(TestMem)*NUM_OBJ)));
		}
	};

	TEST_F(Memory_Speed, RegisterAllocator)
	{
	}

	TEST_F(Memory_Speed, DefaultCpp)
	{
		TestMemMalloc* a[NUM_OBJ];
		for(int it = 0; it < NUM_IT; it++)
		{
			for(int i = 0; i < NUM_OBJ; i++)
			{
				a[i] = new TestMemMalloc();
			}
			for(int i = NUM_OBJ-1; i >= 0; i--)
			{
				delete a[i];
			}
		}
	}

	TEST_F(Memory_Speed, StackObj)
	{
		StackAllocator stack = StackAllocator(sizeof(TestMem)*NUM_OBJ);
		TestMemMalloc* a[NUM_OBJ];
		for(int it = 0; it < NUM_IT; it++)
		{
			for(int i = 0; i < NUM_OBJ; i++)
			{
				a[i] = new (stack.alloc<TestMemMalloc>()) TestMemMalloc();
			}
			for(int i = NUM_OBJ-1; i >= 0; i--)
			{
				stack.free<TestMemMalloc>(a[i]);
			}
		}
	}

	TEST_F(Memory_Speed, StackPtr)
	{
		std::shared_ptr<StackAllocator> stack = MemoryManager::getAllocator<StackAllocator>(SID("Stack"));
		TestMemMalloc* a[NUM_OBJ];
		for(int it = 0; it < NUM_IT; it++)
		{
			for(int i = 0; i < NUM_OBJ; i++)
			{
				a[i] = new (stack->alloc<TestMemMalloc>()) TestMemMalloc();
			}
			for(int i = NUM_OBJ-1; i >= 0; i--)
			{
				stack->free<TestMemMalloc>(a[i]);
			}
		}
	}

	TEST_F(Memory_Speed, StackWithMemoryManager)
	{
		TestMem* a[NUM_OBJ];
		for(int it = 0; it < NUM_IT; it++)
		{
			for(int i = 0; i < NUM_OBJ; i++)
			{
				a[i] = new TestMem();
			}
			for(int i = NUM_OBJ-1; i >= 0; i--)
			{
				delete a[i];
			}
		}
	}
}
