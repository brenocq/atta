#include <atta/ros/include/interface.hpp>
#include <atta/memory/manager.h>
#include <atta/memory/allocator.h>
#include <atta/memory/allocators/bitmapAllocator.h>

#include <atta/event/events/projectOpen.h>
#include <atta/event/events/createComponent.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/events/deleteEntity.h>
#include <atta/component/interface.h>
#include <atta/component/components/components.h>

#define BIND_FREE_EVENT_FUNC(x) \
    (void*)nullptr, std::bind(&x, std::placeholders::_1)
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
    //connect Ros related Events
    event::subscribe<event::ProjectOpen>(BIND_FREE_EVENT_FUNC(ros::openNewProject));
    event::subscribe<event::CreateComponent>(BIND_FREE_EVENT_FUNC(ros::createComponentTopics));
    event::subscribe<event::DeleteComponent>(BIND_FREE_EVENT_FUNC(ros::deleteComponentTopics));
    event::subscribe<event::DeleteEntity>(BIND_FREE_EVENT_FUNC(ros::deleteComponentTopics));
}

void shutDown() {
    node.reset();
}
void update(){
    node->update();
}
void openNewProject(event::Event& event){
    //clear all previous publishers and subscriber to allow for new ones to be created
    node->deleteAllTopics();
}
void createComponentTopics(event::Event& event){
    //get component type and send it to ros
    auto& createCompEvent = static_cast<atta::event::CreateComponent&>(event);

    /*switch (createCompEvent.componentId){


        case 
    }*/
    if(createCompEvent.componentId == component::getId<component::Transform>()){
        ros::node->createTransformTopics(createCompEvent);
    }
    if(createCompEvent.componentId ==  component::getId<component::InfraredSensor>()){
        ros::node->createIRTopics(createCompEvent);
    }

}
void deleteComponentTopics(event::Event& event){
    //get the delete event
    auto& deleteCompEvent = static_cast<atta::event::DeleteComponent&>(event);
    //check that its a transform event
    if(deleteCompEvent.componentId == component::getId<component::Transform>()){
        //delete transform topic for that entity
        ros::node->deleteTransformTopics(deleteCompEvent.entityId);
    }
}   
}