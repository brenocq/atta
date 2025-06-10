#pragma once
#ifdef WITH_ROS2
#include <atta/ros/include/RosPlugin.hpp> //use the main class
#else
#include <atta/ros/include/RosPluginStub.hpp>  // Use the stub instead
#endif

namespace atta::ros {

extern std::shared_ptr<rosPlugin> node;
void startUp();
void shutDown();
void update();


}
