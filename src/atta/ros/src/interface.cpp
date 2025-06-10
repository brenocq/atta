#include <atta/ros/include/interface.hpp>
#include <atta/memory/manager.h>
#include <atta/memory/allocator.h>
#include <atta/memory/allocators/bitmapAllocator.h>
namespace atta::ros {
std::shared_ptr<rosPlugin> node = nullptr;

void startUp() {
    /*
    //----- Module Memory -----//
    // Get main memory
    memory::Allocator* mainAllocator = memory::getAllocator(SSID("MainAllocator"));
    size_t size = 64 * 1024 * 1024; // 64MB
    // Alloc memory inside main memory
    uint8_t* componentMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
    ASSERT(componentMemory != nullptr, "Could not allocate ROS system memory");
    // Create new allocator from component memory
    memory::BitmapAllocator* _allocator = new memory::BitmapAllocator(componentMemory, size);
    memory::registerAllocator(SSID("ros"), static_cast<memory::Allocator*>(_allocator));
    */

    //start the node
    node = std::make_shared<rosPlugin>();
    node->publishData("Started");
}

void shutDown() {
    node.reset();
}
void update(){
    node->update();
}
}
