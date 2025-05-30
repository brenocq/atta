#include "RosPlugin.hpp"
#include <atta/event/interface.h>
#include <atta/event/events/simulationPause.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/events/simulationStep.h>
#include <atta/component/components/transform.h>

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
    deleteAllPubs();
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

void rosPlugin::createTransformPublisher(const atta::event::CreateComponent& event){
    //create topci name
    std::string eName = "E" + std::to_string(event.entityId);
    std::string cName = "C" + std::to_string(event.componentId);
    std::string topic_name = "atta/"+ eName + "/" + cName + "/Odometry";
    int key = event.entityId;
    // make sure if publisher already exists
    if (transformPubs.find(key) == transformPubs.end()){
       
        auto Tpub = node_->create_publisher<geometry_msgs::msg::Pose>(topic_name, 10);
        transformPubs[key] = Tpub;
        LOG_SUCCESS("ros", "Publisher created");
    }else {
        LOG_ERROR("ros", "Publisher creation failed");
    }

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
        LOG_ERROR("ros", "Failed to get transform component for entityId: %d", entityId);
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
            LOG_ERROR("ros", "Exception during publish: %s", e.what());
        } catch (...) {
            LOG_ERROR("ros", "Unknown error while publishing transform msg");
        }
        
    }
}
void rosPlugin::deleteAllPubs(){
    for(auto& [entityId, publisher]: transformPubs){
        if (publisher) {
            publisher.reset();  // Explicitly release the shared_ptr
        }
        transformPubs.clear();
    }

}