#include "RosPlugin.hpp"


ros::ros() {
    // Initialize ROS 2 and create a node and check if ros is not already running
    if (!rclcpp::ok()) { rclcpp::init(0, nullptr); } 
    //create ros node
    node_ = std::make_shared<rclcpp::Node>("ros_plugin_node");

    createPublishers();
    createThread();
    LOG_SUCCESS("[Ros Node]", "Created");

}

ros::~ros() {
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


void ros::publishData(std::string msg) {
    auto message = std_msgs::msg::String();
    message.data = msg;
    publisher_->publish(message);

    //RCLCPP_INFO(node_->get_logger(),"%s", msg.c_str());
}
void ros::createPublishers(){

    // Create a publisher on topic "atta"
    publisher_ = node_->create_publisher<std_msgs::msg::String>("atta", 10);
    RCLCPP_INFO(node_->get_logger(), "ROS Plugin Node Started!");
}
void ros::createThread(){

    // Create and spin executor in separate thread
    executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor_->add_node(node_);
    executor_thread_ = std::thread([this]() {
        executor_->spin();
    });
} 