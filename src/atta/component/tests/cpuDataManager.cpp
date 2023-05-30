//--------------------------------------------------
// Atta Component Tests
// cpuDataManager.cpp
// Date: 2023-05-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/name.h>
#include <atta/component/interface.h>
#include <atta/memory/interface.h>
#include <gtest/gtest.h>

using namespace atta;
using namespace atta::component;

namespace {

class Component_CpuDataManager : public ::testing::Test {
  public:
    void SetUp() {
        uint64_t size = 10.0 * 1024UL * 1024UL; // 10mB
        _mainAllocator = new memory::StackAllocator(size);
        memory::registerAllocator("MainAllocator", static_cast<memory::Allocator*>(_mainAllocator));
        _compAllocator = new memory::BitmapAllocator(size);
        memory::registerAllocator("ComponentAllocator", static_cast<memory::Allocator*>(_compAllocator));
        CpuDataManager::init();

        // Create pools
        for (Registry* r : Registry::get())
            cpuDataManager->allocPool(r->getId(), r->getDescription().maxInstances);
    }

    void TearDown() {
        CpuDataManager::deinit();
        delete _compAllocator;
        delete _mainAllocator;
    }

  private:
    memory::Allocator* _mainAllocator;
    memory::Allocator* _compAllocator;
};

TEST_F(Component_CpuDataManager, Entity) {
    EntityId eid0 = cpuDataManager->createEntity();
    EntityId eid1 = cpuDataManager->createEntity();
    EXPECT_EQ(eid0, 0);
    EXPECT_EQ(eid1, 1);

    cpuDataManager->destroyEntity(eid0);
    eid0 = cpuDataManager->createEntity();
    EXPECT_EQ(eid0, 0);
}

TEST_F(Component_CpuDataManager, Component) {
    Entity e0 = cpuDataManager->createEntity();
    Entity e1 = cpuDataManager->createEntity();
    EXPECT_EQ(e0.getId(), 0);
    EXPECT_EQ(e1.getId(), 1);

    Name* n0 = e0.add<Name>();
    Name* n1 = e1.add<Name>();
    n0->set("E0");
    n1->set("E1");
    EXPECT_EQ(n0 + 1, n1);

    cpuDataManager->destroyEntity(e1);
    EXPECT_EQ(std::string(n0->name), std::string("E0"));
}

} // namespace
