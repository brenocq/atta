//--------------------------------------------------
// Atta Component Tests
// componentPool.cpp
// Date: 2023-05-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/name.h>
#include <atta/component/dataManager/componentPool.h>
#include <gtest/gtest.h>

using namespace atta;
using namespace atta::component;

TEST(Component_ComponentPool, BitmapSize) {
    // Bitmap size should be a multiple of 8
    EXPECT_EQ(ComponentPool::calcBitmapSize(1), 8);
    EXPECT_EQ(ComponentPool::calcBitmapSize(8), 8);
    EXPECT_EQ(ComponentPool::calcBitmapSize(8 * 8), 8);
    EXPECT_EQ(ComponentPool::calcBitmapSize(8 * 8 + 1), 16);
}

TEST(Component_ComponentPool, PoolSize) {
    // Bitmap size should be a multiple of 8
    EXPECT_EQ(ComponentPool::calcPoolSize(1, 10), 8 + 10);
    EXPECT_EQ(ComponentPool::calcPoolSize(8, 20), 8 + 8 * 20);
    EXPECT_EQ(ComponentPool::calcPoolSize(8 * 8, 32), 8 + 32 * 8 * 8);
    EXPECT_EQ(ComponentPool::calcPoolSize(8 * 8 + 1, 1), 16 + 8 * 8 + 1);
}

TEST(Component_ComponentPool, AllocFree) {
    size_t size = 8;
    uint8_t data[sizeof(Name) * size] = {};
    ComponentPool p = ComponentPool(data, sizeof(data), sizeof(Name), size);
    Name* n0 = (Name*)p.alloc();
    Name* n1 = (Name*)p.alloc();
    n0->set("Name0");
    n1->set("Name1");
    EXPECT_EQ(n0 + 1, n1);
    EXPECT_EQ(std::string(n0->name), std::string("Name0"));
    p.free(n1);
    EXPECT_EQ(std::string(n0->name), std::string("Name0"));
}
