#include <atta/ros/include/interface.hpp>
#include <atta/memory/manager.h>
#include <atta/memory/allocator.h>
#include <atta/memory/allocators/bitmapAllocator.h>
#include <atta/utils/log.h>
#include <atta/event/events/projectOpen.h>
#include <atta/event/events/createComponent.h>
#include <atta/event/events/deleteComponent.h>
#include <atta/event/events/deleteEntity.h>
#include <atta/component/interface.h>
#include <atta/component/components/components.h>
#include <atta/component/components/rigidBody.h>

#define BIND_FREE_EVENT_FUNC(x) \
    (void*)nullptr, std::bind(&x, std::placeholders::_1)
namespace atta::ros {
std::shared_ptr<rosPlugin> node = nullptr;

template<class T, class... Args>
std::shared_ptr<T> bitmapAllocShared(atta::memory::BitmapAllocator* alloc, Args&&... args) {
    size_t blockSize = alloc->getBlockSize();
    size_t sz = sizeof(T);
    size_t rounded = ((sz + blockSize - 1) / blockSize) * blockSize;
    void* mem = alloc->allocBytes(rounded, alignof(T));
    if(!mem) throw std::bad_alloc();
    T* obj = new (mem) T(std::forward<Args>(args)...);
    auto deleter = [alloc, rounded](T* p){
        if(p) { p->~T(); alloc->freeBytes(p, rounded, alignof(T)); }
    };
    return std::shared_ptr<T>(obj, deleter);
}

void startUp() {
    
    //----- Module Memory -----//
    // Get main memory
    memory::Allocator* mainAllocator = memory::getAllocator(SSID("MainAllocator"));
    // 1. Stack for per-callback temporary data (FASTEST)
    /*size_t stackSize = 20 * 1024 * 1024; // 20MB
    uint8_t* stackMem =  static_cast<uint8_t*>(mainAllocator->allocBytes(stackSize, sizeof(uint8_t)));
    memory::StackAllocator* _stackAllocator = new memory::StackAllocator(stackMem, stackSize);
    memory::registerAllocator(SSID("ROSStack"), _stackAllocator);
    
    // 2. Pool for messages (MOST EFFICIENT for fixed-size)
    size_t poolSize = 5 * 1024 * 1024; // 5MB
    uint8_t* poolMem = static_cast<uint8_t*>(mainAllocator->allocBytes(poolSize, sizeof(uint8_t)));
    memory::PoolAllocator* _messagePool = new memory::PoolAllocator(poolMem, 1000, 256, 256);
    memory::registerAllocator(SSID("ROSMessagePool"), _messagePool);
    */
    // 3. Bitmap for buffers (MOST FLEXIBLE)
    size_t bitmapSize = 100 * 1024 * 1024; // 100MB
    uint8_t* bitmapMem = static_cast<uint8_t*>(mainAllocator->allocBytes(bitmapSize, sizeof(uint8_t)));
    memory::BitmapAllocator* _bufferAllocator = new memory::BitmapAllocator(bitmapMem, bitmapSize, 4096);
    memory::registerAllocator(SSID("ROSBuffers"), _bufferAllocator);
    

    //start the node
    node = bitmapAllocShared<rosPlugin>(_bufferAllocator);
    //node = std::make_shared<rosPlugin>();
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
    if(createCompEvent.componentId == component::getId<component::Transform>()){               //transform compoenet topic creation
        ros::node->createTransformTopics(createCompEvent);
    }else if(createCompEvent.componentId ==  component::getId<component::InfraredSensor>()){   //IR compoenet topic creation
        ros::node->createIRTopics(createCompEvent);
    }else if (createCompEvent.componentId ==  component::getId<component::RigidBody>()){       //Rigid body compoenet topic creation
        ros::node->createRigidTopics(createCompEvent);
    }else if (createCompEvent.componentId ==  component::getId<component::CameraSensor>()){     //CameraS  compoenet topic creation
        //ros::node->;
    }


    else {
        ros::node->publishData("component Id not recognized, cannot create topic");
        LOG_ERROR("Ros", "component Id not recognized, cannot create topic");
    }

}
void deleteComponentTopics(event::Event& event){
    //get the delete event
    auto& deleteCompEvent = static_cast<atta::event::DeleteComponent&>(event);
    //check that its a transform event
    if(deleteCompEvent.componentId == component::getId<component::Transform>()){                //Transform compoenet topic Deletion
        ros::node->deleteTransformTopics(deleteCompEvent.entityId);
    }else if(deleteCompEvent.componentId ==  component::getId<component::InfraredSensor>()){    //IR compoenet topic Deletion
        ros::node->deleteIRTopics(deleteCompEvent.entityId);
    }else if (deleteCompEvent.componentId ==  component::getId<component::RigidBody>()){       //Rigid body compoenet topic creation
        ros::node->deleteRigidTopics(deleteCompEvent.entityId);
    }
    else {
        ros::node->publishData("component Id not recognized, cannot delete topic");
        LOG_ERROR("Ros", "component Id not recognized, cannot delete topic");
    }
}   
}