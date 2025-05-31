#include "RosPlugin.hpp"
#include <atta/event/interface.h>
#include <atta/event/events/simulationPause.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/events/simulationStep.h>
#include <atta/component/components/transform.h>
#include <atta/component/components/name.h>
#include "Util.hpp"
#include <exception>

rosPlugin::rosPlugin() {
    // Initialize ROS 2 and create a node and check if ros is not already running
    if (!rclcpp::ok()) { rclcpp::init(0, nullptr); rosInitializedHere=true; } 
    //create ros node
    node_ = std::make_shared<rclcpp::Node>("ros_plugin_node");

    createPublishers();
    createThread();

    //Simulation Services
    pausePhysics = node_->create_service<std_srvs::srv::Trigger>(
                    "atta/Simulation_Control/pause_physics",
                    std::bind(&rosPlugin::pauseCallback,this,std::placeholders::_1, std::placeholders::_2));
    unPausePhysics = node_->create_service<std_srvs::srv::Trigger>(
                    "atta/Simulation_Control/unPause_physics",
                    std::bind(&rosPlugin::unPauseCallback,this,std::placeholders::_1, std::placeholders::_2));
    resetSimulation = node_->create_service<std_srvs::srv::Trigger>(
                    "atta/Simulation_Control/reset_physics",
                    std::bind(&rosPlugin::resetCallback,this,std::placeholders::_1, std::placeholders::_2));
    stepSimulation = node_->create_service<std_srvs::srv::Trigger>(
                    "atta/Simulation_Control/step_Simulation",
                    std::bind(&rosPlugin::stepCallback,this,std::placeholders::_1, std::placeholders::_2));
    //___

    LOG_SUCCESS("Ros Node", "Created");
}

rosPlugin::~rosPlugin() {
    //clean up
    if (executor_) {
        executor_->cancel();
    }
    if (executor_thread_.joinable()) {
        executor_thread_.join();
    }
    deleteAllTopics();
    node_.reset();
    executor_.reset();
    if (rclcpp::ok() && rosInitializedHere) {
        rclcpp::shutdown();
    }
}


void rosPlugin::publishData(std::string msg) {
    auto message = std_msgs::msg::String();
    message.data = msg;
    publisher_->publish(message);

    //RCLCPP_INFO(node_->get_logger(),"%s", msg.c_str());
}
void rosPlugin::createPublishers(){

    // Create a publisher on topic "atta"
    publisher_ = node_->create_publisher<std_msgs::msg::String>("atta", 10);
    RCLCPP_INFO(node_->get_logger(), "ROS Plugin Node Started!");
}
void rosPlugin::createThread(){

    // Create and spin executor in separate thread
    executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor_->add_node(node_);
    executor_thread_ = std::thread([this]() {
        executor_->spin();
    });
} 
void rosPlugin::update(){
    this->updateTransform();
}

// Simulation Control
void rosPlugin::pauseCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                            std::shared_ptr<std_srvs::srv::Trigger::Response> response){
    try{
    // atta side
    atta::event::SimulationPause pauseEvent;
    atta::event::publish(pauseEvent);
    // ros side
    response->success = true;
    response->message = "Simulation paused successfully";
    }
    catch (...){
        response->success = false;
        response->message = "Failed to pause simulation.";
    }
    }
void rosPlugin::unPauseCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                            std::shared_ptr<std_srvs::srv::Trigger::Response> response){
    try{
    // atta side
    atta::event::SimulationStart startEvent;
    atta::event::publish(startEvent);
    // ros side
    response->success = true;
    response->message = "Simulation resumed successfully";
    }
    catch (...){
        response->success = false;
        response->message = "Failed to resume simulation.";
    }
    }
void rosPlugin::resetCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                            std::shared_ptr<std_srvs::srv::Trigger::Response> response){
    try{
    // atta side
    atta::event::SimulationStop stopEvent;
    atta::event::publish(stopEvent);
    // ros side
    response->success = true;
    response->message = "Simulation stopped successfully";
    }
    catch (...){
        response->success = false;
        response->message = "Failed to stop simulation.";
    }
    }     
void rosPlugin::stepCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                            std::shared_ptr<std_srvs::srv::Trigger::Response> response){
    try{
    // atta side
    atta::event::SimulationStep stepSim;
    atta::event::publish(stepSim);
    // ros side
    response->success = true;
    response->message = "Simulation stepped successfully";
    }
    catch (...){
        response->success = false;
        response->message = "Failed to step simulation.";
    }
}

void rosPlugin::createTransformTopics(const atta::event::CreateComponent& event){
    int key = event.entityId;
    
    //create topic name
    auto* cName = atta::component::getComponent<atta::component::Name>(key);
    std::string eName = cName? std::string(cName->name) : "E" + std::to_string(key);
    removeSpacesFrom(eName);
    std::string pub_Topic_name = "atta/"+ eName + "/transform/Odometry";
    std::string sub_Topic_name = "atta/"+ eName + "/transform/Set";
    
    // make sure if publisher already exists, if not create it
    try{
    if (transformPubs.find(key) == transformPubs.end()){
        auto Tpub = node_->create_publisher<geometry_msgs::msg::Pose>(pub_Topic_name, 10);
        transformPubs[key] = Tpub;
    }else {
        LOG_ERROR("Ros", "Publisher creation failed; Key " + std::to_string(key) + "already exists.");
    }}catch(const std::exception& e){
        LOG_ERROR("Ros",std::string("error creating Publisher: ") + e.what());
    }

    // make sure if subscriber already exists, if not create it
    try{
    if (transformSubs.find(key) == transformSubs.end()){
        auto Tsub = node_->create_subscription<geometry_msgs::msg::Pose>(sub_Topic_name, 10,
                                                                         [this,key](geometry_msgs::msg::Pose::SharedPtr  msg){
                                                                            this->transformCallback(msg, key);}
                                                                         );
        transformSubs[key] = Tsub;
    }else {
        LOG_ERROR("Ros", "Subscriber creation failed");
    }}catch(const std::exception& e){
        LOG_ERROR("Ros",std::string("error creating Subscriber: ") + e.what());
    }
    LOG_SUCCESS("Ros", std::string("Transform Topics created for ID:") + std::to_string(key));
}
void rosPlugin::updateTransform(){

    for(const auto& [entityId, publisher]: transformPubs){
        //check that the publisher has subscribers
        if (publisher->get_subscription_count() == 0){
            continue;
        }
        //get transform component from entity
        auto* transform = atta::component::getComponent<atta::component::Transform>(entityId);
        if (!transform){
        LOG_ERROR("ros", std::string( "Failed to get transform component for entityId: ") + std::to_string(entityId));
        continue;
        }
        //cast transform data in ros msg format
        atta::component::Transform data = transform->getWorldTransform(entityId);
        geometry_msgs::msg::Pose msg;
        msg.position.x = data.position.x;
        msg.position.y = data.position.y;
        msg.position.z = data.position.z;

        msg.orientation.w = data.orientation.r;
        msg.orientation.x = data.orientation.i;
        msg.orientation.y = data.orientation.j;
        msg.orientation.z = data.orientation.k; 
        //publish msg
        try{
            publisher->publish(msg);
        }
        catch (const std::exception& e) {
            LOG_ERROR("ros", std::string("Exception during publish: ") + e.what());
        } catch (...) {
            LOG_ERROR("ros", "Unknown error while publishing transform msg");
        }
        
    }
}
void rosPlugin::deleteAllTopics(){
    for(auto& [entityId, publisher]: transformPubs){
        if (publisher) {
            publisher.reset();  // Explicitly release the shared_ptr
        }
        transformPubs.clear();
    }
    for(auto& [entityId, subscriber]: transformSubs){
        if (subscriber) {
            subscriber.reset();  // Explicitly release the shared_ptr
        }
        transformSubs.clear();
    }

}
bool rosPlugin::deleteTransformTopics(int id){
    if (transformPubs.find(id) != transformPubs.end()){
            transformPubs.erase(id);
    }else{
        LOG_ERROR("Ros", "Failed to delete Transform Publisher for id: " + std::to_string(id) + " or id doesnt exist");
        return false;
    }
    if (transformSubs.find(id) != transformSubs.end()){
            transformSubs.erase(id);
    }else{
        LOG_ERROR("Ros", "Failed to delete Subscriber Publisher for id: " + std::to_string(id) + " or id doesnt exist");
        return false;
    }
    LOG_SUCCESS("Ros", "Transform Topics deleted for id: " + std::to_string(id));
    return true;
}
void rosPlugin::transformCallback(const geometry_msgs::msg::Pose::SharedPtr msg, int key) const{

    //get transform component from entity
    auto* transform = atta::component::getComponent<atta::component::Transform>(key);
    //cast ros msg format to transform data format
    atta::component::Transform data;
    data.position.x = msg->position.x;
    data.position.y = msg->position.y;
    data.position.z = msg->position.z;

    data.orientation.r = msg->orientation.w;
    data.orientation.i = msg->orientation.x;
    data.orientation.j = msg->orientation.y;
    data.orientation.k = msg->orientation.z;
    //send data to transform object
    transform->setWorldTransform(key, data);

}