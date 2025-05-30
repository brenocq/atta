#ifndef ROS_PLUGIN_STUB_HPP
#define ROS_PLUGIN_STUB_HPP


#include <thread>
#include <memory>
#include <atta/event/event.h>
#include <atta/event/events/createComponent.h>

class rosPlugin {
public:
    rosPlugin();
    ~rosPlugin();

    void update();
    void publishData(std::string msg);
    void createTransformPublisher(const atta::event::CreateComponent& event);
    bool deleteTransformPub(int id);
private:
    void createPublishers();
    void createServices();

    void createThread();
    
};

#endif // ROS_PLUGIN_STUB_HPP
