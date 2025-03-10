#ifndef ROS_PLUGIN_STUB_HPP
#define ROS_PLUGIN_STUB_HPP


#include <thread>
#include <memory>


class RosPlugin {
public:
    RosPlugin();
    ~RosPlugin();

    //void update();

private:
    void createPublishers();
    void createServices();
    void publishData();
    void createThread();

};

#endif // ROS_PLUGIN_STUB_HPP
