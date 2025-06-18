#include "RosPluginStub.hpp"
#include "Util.hpp"
namespace atta::ros {

rosPlugin::rosPlugin() {}
rosPlugin::~rosPlugin() {}

    void update();
    void publishData(std::string msg){}
    void createTransformTopics(const atta::event::CreateComponent& event){}
    bool deleteTransformTopics(int id){}
    void createIRTopics(const atta::event::CreateComponent& event){}
    void deleteIRTopics(int id){}
    void createPublishers(){}
    void createServices(){}
    void createThread(){}
}