//--------------------------------------------------
// Atta
// atta.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ATTA_H
#define ATTA_ATTA_H
#include <atta/event/event.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <ctime>

//check if ROS2 is enabled
#ifdef WITH_ROS2
#include <atta/ros/include/RosPlugin.hpp>
#else
#include <atta/ros/include/RosPluginStub.hpp>  // Use the stub instead
#endif
namespace atta {
class Atta {
  public:
    struct CreateInfo {
        std::filesystem::path projectFile = "";
    };

    Atta(const CreateInfo& info);
    ~Atta();

    void run();
    void loop();
    void step();

  private:
    // Handle events
    void onWindowClose(event::Event& event);
    void onSimulationStateChange(event::Event& event);
    void createComponentTopics(event::Event& event);
    void deleteComponentTopics(event::Event& event);
    // Memory
    memory::StackAllocator* _mainAllocator;

    // State
    bool _shouldFinish;
    bool _shouldStep;
    clock_t _lastStep;
    clock_t _currStep;
    
    std::shared_ptr<rosPlugin> ros_node;
};
} // namespace atta

#endif // ATTA_ATTA_H
