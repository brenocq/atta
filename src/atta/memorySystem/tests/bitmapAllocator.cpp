//--------------------------------------------------
// Atta Memory Tests
// bitmapAllocator.cpp
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/memorySystem/allocators/bitmapAllocator.h>

using namespace atta;
namespace
{
    TEST(Memory_BitmapAllocator, DataMemorySize)
    {
        BitmapAllocator a0(1);
        BitmapAllocator a1(2);
        BitmapAllocator a2(8);
        BitmapAllocator a3(9);
        BitmapAllocator a4(1000);

        EXPECT_EQ(a1.getDataSize(), 1);
        EXPECT_EQ(a2.getDataSize(), 7);
        EXPECT_EQ(a3.getDataSize(), 8);
        EXPECT_EQ(a4.getDataSize(), 888);
    }

    TEST(Memory_BitmapAllocator, Allocation)
    {
        {
            BitmapAllocator a(9);
            // Memory: x--------
            int* i0 = a.alloc<int>(1);// 4 bytes
            // Memory: xaaaa----
            int* i1 = a.alloc<int>(1);// 4 bytes
            // Memory: xaaaaaaaa
            int* i2 = a.alloc<int>(1);// nullptr

            EXPECT_NE(i0, nullptr);
            EXPECT_NE(i1, nullptr);
            EXPECT_EQ(i2, nullptr);
        }

        {
            // Alloc multiple
            BitmapAllocator a(9);
            int* i0 = a.alloc<int>(2);// 8 bytes
            EXPECT_NE(i0, nullptr);
        }

        {
            // Alloc multiple
            BitmapAllocator a(9);
            int* i0 = a.alloc<int>(3);// 12 bytes
            EXPECT_EQ(i0, nullptr);
        }

        {
            BitmapAllocator a(12);
            // Memory: xx----------
            int* i0 = a.alloc<int>(1);
            // Memory: xxaaaa------
            int* i1 = a.alloc<int>(1);
            // Memory: xxaaaaaaaa--
            int* i2 = a.alloc<int>(1);
            uint16_t* i3 = a.alloc<uint16_t>(1);

            EXPECT_NE(i0, nullptr);
            EXPECT_NE(i1, nullptr);
            EXPECT_EQ(i2, nullptr);
            EXPECT_NE(i3, nullptr);
        }

        {
            BitmapAllocator a(2);
            // Memory: x-
            int* i0 = a.alloc<int>(1);
            char* c0 = a.alloc<char>(1);

            EXPECT_EQ(i0, nullptr);
            EXPECT_NE(c0, nullptr);
        }
    }

    TEST(Memory_BitmapAllocator, Free)
    {
        {
            BitmapAllocator a(9);
            // Memory: x--------
            int* i0 = a.alloc<int>(1);
            // Memory: xaaaa----
            int* i1 = a.alloc<int>(1);
            a.free<int>(i1);
            // Memory: xaaaa----
            int* i2 = a.alloc<int>(1);
            // Memory: xaaaaaaaa

            EXPECT_NE(i0, nullptr);
            EXPECT_NE(i2, nullptr);
        }

        {
            BitmapAllocator a(9);
            // Memory: x--------
            int* i0 = a.alloc<int>();
            // Memory: x0000----
            int* i1 = a.alloc<int>();
            // Memory: x00001111
            a.free<int>(i0);
            // Memory: x----1111
            char* c2 = a.alloc<char>(2);
            // Memory: x22--1111
            char* c3 = a.alloc<char>(2);
            // Memory: x22331111
            // Current:     ^

            EXPECT_NE(i1, nullptr);
            EXPECT_NE(c2, nullptr);
            EXPECT_NE(c3, nullptr);

            *i1 = 1;
            *c2 = 'a';
            *c3 = 'b';

            EXPECT_EQ(*i1, 1);
            EXPECT_EQ(*c2, 'a');
            EXPECT_EQ(*c3, 'b');

            a.free<int>(i1);
            a.free<char>(c2, 2);
            // Memory: x--33----
            // Current:     ^

            c2 = a.alloc<char>(5);
            // Memory: x--33----
            // Current:     ^

            a.free<char>(c3, 2);
            // Memory: x--------
            // Current:     ^

            double* l4 = a.alloc<double>(2);
            // Memory: x--------
            // Current:     ^
            EXPECT_EQ(l4, nullptr);

            l4 = a.alloc<double>(1);
            // Memory: xaaaaaaaa
            // Current:     ^
            EXPECT_NE(l4, nullptr);
        }
    }
}
