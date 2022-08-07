//--------------------------------------------------
// Atta Memory Tests
// poolAllocator.cpp
// Date: 2021-08-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/memory/allocators/poolAllocatorT.h>
#include <gtest/gtest.h>

using namespace atta::memory;
namespace {
class TestObject {
  public:
    TestObject(int startVal) { x[0] = y[0] = z[0] = startVal; }

    uint8_t x[100];
    uint8_t y[100];
    uint8_t z[100];
};

TEST(Memory_PoolAllocator, Constructors) {
    // No external memory, no alignment
    {
        PoolAllocatorT<int> pool(2);
        int* i0 = pool.alloc();
        int* i1 = pool.alloc();
        int* i2 = pool.alloc();

        EXPECT_NE(i0, nullptr);
        EXPECT_NE(i1, nullptr);
        EXPECT_EQ(i2, nullptr);
        if (i0 == nullptr || i1 == nullptr)
            GTEST_SKIP();

        *i0 = 10;
        *i1 = 20;

        EXPECT_EQ(*i0, 10);
        EXPECT_EQ(*i1, 20);
    }

    // External memory, no alignment
    {
        uint8_t* mem = static_cast<uint8_t*>(malloc(10 * sizeof(int)));
        PoolAllocatorT<int> pool(mem, 2);
        int* i0 = pool.alloc();
        int* i1 = pool.alloc();
        int* i2 = pool.alloc();

        EXPECT_NE(i0, nullptr);
        EXPECT_NE(i1, nullptr);
        EXPECT_EQ(i2, nullptr);
        if (i0 == nullptr || i1 == nullptr)
            GTEST_SKIP();

        *i0 = 10;
        *i1 = 20;

        EXPECT_EQ(*i0, 10);
        EXPECT_EQ(*i1, 20);
    }
}

TEST(Memory_PoolAllocator, BigBlock) {
    PoolAllocatorT<TestObject> pool(2);
    TestObject* o0 = pool.alloc();
    TestObject* o1 = pool.alloc();
    TestObject* o2 = pool.alloc();

    EXPECT_NE(o0, nullptr);
    EXPECT_NE(o1, nullptr);
    EXPECT_EQ(o2, nullptr);
    if (o0 == nullptr || o1 == nullptr)
        GTEST_SKIP();

    o0->x[0] = 10;
    o1->y[10] = 20;

    EXPECT_EQ(o0->x[0], 10);
    EXPECT_EQ(o1->y[10], 20);
}

TEST(Memory_PoolAllocator, PoolReuse) {
    PoolAllocatorT<int> pool(2);
    int* i0 = pool.alloc();
    int* i1 = pool.alloc();
    int* i2 = pool.alloc();

    EXPECT_NE(i0, nullptr);
    EXPECT_NE(i1, nullptr);
    EXPECT_EQ(i2, nullptr);
    if (i0 == nullptr || i1 == nullptr)
        GTEST_SKIP();

    *i0 = 10;
    *i1 = 20;

    pool.free(i0);
    i2 = pool.alloc();
    *i2 = 30;

    EXPECT_NE(i2, nullptr);
    if (i2 == nullptr)
        GTEST_SKIP();
    EXPECT_EQ(*i0, 30);
    EXPECT_EQ(*i2, 30);
}

TEST(Memory_PoolAllocator, Clear) {
    PoolAllocatorT<int> pool(4);
    int* i0 = pool.alloc();
    int* i1 = pool.alloc();

    *i0 = 10;
    *i1 = 20;

    pool.clear();

    int* i2 = pool.alloc();
    int* i3 = pool.alloc();

    *i2 = 30;
    *i3 = 40;

    EXPECT_EQ(*i2, 30);
    EXPECT_EQ(*i3, 40);
}
} // namespace
