#pragma once
#ifdef WITH_ROS2
#include <atta/ros/include/RosPlugin.hpp> //use the main class
#else
#include <atta/ros/include/RosPluginStub.hpp>  // Use the stub instead
#endif
#include <atta/event/event.h>

namespace atta::ros {

extern std::shared_ptr<rosPlugin> node;
void startUp();
void shutDown();
void update();
void openNewProject(event::Event& event);
void createComponentTopics(event::Event& event);
void deleteComponentTopics(event::Event& event);
}
