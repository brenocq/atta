#ifndef ROS_PLUGIN_STUB_HPP
#define ROS_PLUGIN_STUB_HPP


#include <thread>
#include <memory>


class ros {
public:
    ros();
    ~ros();

    //void update();
    void publishData(std::string msg);
private:
    void createPublishers();
    void createServices();

    void createThread();

};

#endif // ROS_PLUGIN_STUB_HPP
