#include "RosPlugin.hpp"
#include <atta/event/event.h>
#include <atta/event/interface.h>
#include <atta/event/events/simulationPause.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>

rosPlugin::rosPlugin() {
    // Initialize ROS 2 and create a node and check if ros is not already running
    if (!rclcpp::ok()) { rclcpp::init(0, nullptr); } 
    //create ros node
    node_ = std::make_shared<rclcpp::Node>("ros_plugin_node");

    createPublishers();
    createThread();

    //Simulation Services
    pausePhysics = node_->create_service<std_srvs::srv::Trigger>(
                    "Simulation_Control/pause_physics",
                    std::bind(&rosPlugin::pauseCallback,this,std::placeholders::_1, std::placeholders::_2));
    unPausePhysics = node_->create_service<std_srvs::srv::Trigger>(
                    "Simulation_Control/unPause_physics",
                    std::bind(&rosPlugin::unPauseCallback,this,std::placeholders::_1, std::placeholders::_2));
    resetSimulation = node_->create_service<std_srvs::srv::Trigger>(
                    "Simulation_Control/reset_physics",
                    std::bind(&rosPlugin::resetCallback,this,std::placeholders::_1, std::placeholders::_2));
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

    if (rclcpp::ok()) {
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