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
#include <atta/memory/allocators/mallocAllocator.h>

using namespace atta;
namespace
{
    constexpr int NUM_IT = 1000;
    constexpr int NUM_OBJ = 5000;

    struct TestStack : public AllocatedObject<TestStack, SID("Stack")>
    {
        int x, y, z;
    };

    struct TestMalloc : public AllocatedObject<TestMalloc, SID("Malloc")>
    {
        int x, y, z;
    };

    struct TestCpp
    {
        int x, y, z;
    };

    class Memory_Speed : public ::testing::Test
    {
    public:
        void SetUp()
        {
            MemoryManager::registerAllocator(
                    SID("Stack"), 
                    static_cast<Allocator*>(new StackAllocator(sizeof(TestStack)*NUM_OBJ)));

            MemoryManager::registerAllocator(
                    SID("Malloc"), 
                    static_cast<Allocator*>(new MallocAllocator()));
        }
    };

    TEST_F(Memory_Speed, DefaultNewCpp)
    {
        TestCpp* a[NUM_OBJ];
        for(int it = 0; it < NUM_IT; it++)
        {
            for(int i = 0; i < NUM_OBJ; i++)
            {
                a[i] = new TestCpp();
            }
            for(int i = NUM_OBJ-1; i >= 0; i--)
            {
                delete a[i];
            }
        }
    }

    TEST_F(Memory_Speed, MallocWithMemoryManager)
    {
        TestMalloc* a[NUM_OBJ];
        for(int it = 0; it < NUM_IT; it++)
        {
            for(int i = 0; i < NUM_OBJ; i++)
            {
                a[i] = new TestMalloc();
            }
            for(int i = NUM_OBJ-1; i >= 0; i--)
            {
                delete a[i];
            }
        }
    }

    TEST_F(Memory_Speed, StackObj)
    {
        StackAllocator stack = StackAllocator(sizeof(TestStack)*NUM_OBJ);
        TestStack* a[NUM_OBJ];
        for(int it = 0; it < NUM_IT; it++)
        {
            for(int i = 0; i < NUM_OBJ; i++)
            {
                a[i] = new (stack.alloc<TestStack>()) TestStack();
            }
            for(int i = NUM_OBJ-1; i >= 0; i--)
            {
                stack.free<TestStack>(a[i]);
            }
        }
    }

    TEST_F(Memory_Speed, StackPtr)
    {
        StackAllocator* stack = MemoryManager::getAllocator<StackAllocator>(SID("Stack"));
        TestStack* a[NUM_OBJ];
        for(int it = 0; it < NUM_IT; it++)
        {
            for(int i = 0; i < NUM_OBJ; i++)
            {
                a[i] = new (stack->alloc<TestStack>()) TestStack();
            }
            for(int i = NUM_OBJ-1; i >= 0; i--)
            {
                stack->free<TestStack>(a[i]);
            }
        }
        EXPECT_EQ(stack->getUsedMemory(), 0);
    }

    TEST_F(Memory_Speed, StackWithMemoryManager)
    {
        TestStack* a[NUM_OBJ];
        for(int it = 0; it < NUM_IT; it++)
        {
            for(int i = 0; i < NUM_OBJ; i++)
            {
                a[i] = new TestStack();
            }
            for(int i = NUM_OBJ-1; i >= 0; i--)
            {
                delete a[i];
            }
        }

        StackAllocator* stack = MemoryManager::getAllocator<StackAllocator>(SID("Stack"));
        EXPECT_EQ(stack->getUsedMemory(), 0);
    }
}
