//--------------------------------------------------
// Atta Memory Tests
// allocatedObject.cpp
// Date: 2021-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocatedObject.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <atta/memory/interface.h>
#include <atta/utils/stringId.h>
#include <gtest/gtest.h>

using namespace atta;
using namespace atta::memory;

namespace {
class Memory_AllocatedObject : public ::testing::Test {
  public:
    void SetUp() {
        memory::registerAllocator(SID("Stack_TestA"), static_cast<Allocator*>(new StackAllocator(sizeof(int) * 3)));

        memory::registerAllocator(SID("Stack_TestB"), static_cast<Allocator*>(new StackAllocator(sizeof(int) * 2)));
    }
};

class TestA : public AllocatedObject<TestA, SID("Stack_TestA")> {
  public:
    TestA(int val) : x(val) {}
    TestA() : x(1) {}
    int x;
};

class TestB : public AllocatedObject<TestB, SID("Stack_TestB")> {
  public:
    TestB(int val) : x(val) {}
    int x;
};

TEST_F(Memory_AllocatedObject, New) {
    //----- Test new -----//
    TestA* a0 = new TestA(0);
    EXPECT_EQ(a0->x, 0);

    TestA* a1 = new (std::nothrow) TestA(1);
    EXPECT_EQ(a1->x, 1);

    TestA* a2 = memory::getAllocator<StackAllocator>(SID("Stack_TestA"))->alloc<TestA>();
    new (a2) TestA(2);
    EXPECT_EQ(a2->x, 2);

    TestB* b0 = new TestB(4);
    EXPECT_EQ(b0->x, 4);

    //----- Check stack size -----//
    StackAllocator* stackA = memory::getAllocator<StackAllocator>(SID("Stack_TestA"));
    EXPECT_EQ(stackA->getSize(), sizeof(int) * 3);
    EXPECT_EQ(stackA->getUsedMemory(), sizeof(int) * 3);

    StackAllocator* stackB = memory::getAllocator<StackAllocator>(SID("Stack_TestB"));
    EXPECT_EQ(stackB->getSize(), sizeof(int) * 2);
    EXPECT_EQ(stackB->getUsedMemory(), sizeof(int));
}

TEST_F(Memory_AllocatedObject, Delete) {
    StackAllocator* stackA = memory::getAllocator<StackAllocator>(SID("Stack_TestA"));
    stackA->clear();
    EXPECT_EQ(stackA->getSize(), sizeof(int) * 3);
    EXPECT_EQ(stackA->getUsedMemory(), 0);

    TestA *a0, *a1, *a2;
    a0 = new TestA(0);
    EXPECT_EQ(stackA->getUsedMemory(), sizeof(TestA));
    a1 = new TestA(1);
    a2 = new TestA(2);

    delete a0; // Should not be deleted
    delete a1; // Should not be deleted
    delete a2; // Deleted
    EXPECT_EQ(stackA->getUsedMemory(), sizeof(int) * 2);

    // Finish deleting
    delete a1;
    delete a0;
    EXPECT_EQ(stackA->getUsedMemory(), 0);
}

TEST_F(Memory_AllocatedObject, NewArray) {
    EXPECT_THROW(new TestA[10], std::bad_alloc);
    TestA* a = new TestA[3];
    a[0].x = 10;
    EXPECT_EQ(a->x, 10);
    EXPECT_EQ(a[1].x, 1);
    EXPECT_EQ(a[2].x, 1);

    StackAllocator* stackA = memory::getAllocator<StackAllocator>(SID("Stack_TestA"));
    stackA->clear();
}

TEST_F(Memory_AllocatedObject, DeleteArray) {
    StackAllocator* stackA = memory::getAllocator<StackAllocator>(SID("Stack_TestA"));
    TestA* a0 = new TestA(0);

    TestA* a = new TestA[2];
    // To be more memory efficient, the stack does not store the size of the array.
    // Thus, the delete[] does not work and we must return stack pointer explicitly.
    // delete[] a;
    stackA->free(a, 2);

    TestA* a1 = new TestA(1);
    TestA* a2 = new TestA(2);
    delete a2;
    delete a1;

    delete a0;

    EXPECT_EQ(stackA->getUsedMemory(), 0);
}
} // namespace
