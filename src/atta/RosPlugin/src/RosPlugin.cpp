#include "RosPlugin.hpp"


RosPlugin::RosPlugin() {
    // Initialize ROS 2 and create a node and check if ros is not already running
    if (!rclcpp::ok()) { rclcpp::init(0, nullptr); } 
    //create ros node
    node_ = std::make_shared<rclcpp::Node>("ros_plugin_node");

    createPublishers();
    createThread();
    timer_ = node_->create_wall_timer(
        std::chrono::seconds(2), 
        std::bind(&RosPlugin::publishData, this)
    );

    LOG_SUCCESS("Node", "Created");
    publishData();   

}

RosPlugin::~RosPlugin() {
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


void RosPlugin::publishData() {
    auto message = std_msgs::msg::String();
    message.data = "Hello from RosPlugin!";
    publisher_->publish(message);
    LOG_SUCCESS("Node", "publishData");
    RCLCPP_INFO(node_->get_logger(), "ROS publishData");
}

void RosPlugin::createPublishers(){

    // Create a publisher on topic "atta"
    publisher_ = node_->create_publisher<std_msgs::msg::String>("atta", 10);
    RCLCPP_INFO(node_->get_logger(), "ROS Plugin Node Started!");
}
void RosPlugin::createThread(){

    // Create and spin executor in separate thread
    executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor_->add_node(node_);
    executor_thread_ = std::thread([this]() {
        executor_->spin();
    });
} 