#ifndef ROS_PLUGIN_STUB_HPP
#define ROS_PLUGIN_STUB_HPP


#include <thread>
#include <memory>
#include <atta/event/event.h>
#include <atta/event/events/createComponent.h>
namespace atta::ros {
class rosPlugin {
public:
    rosPlugin();
    ~rosPlugin();

    void update();
    void publishData(std::string msg);
    void createTransformTopics(const atta::event::CreateComponent& event);
    void deleteTransformTopics(int id);
    void createIRTopics(const atta::event::CreateComponent& event);
    void deleteIRTopics(int id);
    void deleteAllTopics();
    void createRigidTopics(const atta::event::CreateComponent& event);
    void deleteRigidTopics(int id);
private:
    void createPublishers();
    void createServices();
    void createThread();
    
};
}
#endif // ROS_PLUGIN_STUB_HPP
