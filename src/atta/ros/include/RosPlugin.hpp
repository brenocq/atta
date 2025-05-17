#ifndef ROS_PLUGIN_HPP
#define ROS_PLUGIN_HPP

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <thread>
#include <memory>
#include <atta/utils/config.h>
#include <rclcpp/executors/single_threaded_executor.hpp>

class ros {
public:
    ros();
    ~ros();

    //void update();
    void publishData(std::string msg);
private:
    void createPublishers();
    void createServices();

    void deletePublisher();
    void createThread();
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor_;
    std::thread executor_thread_;
    std::vector<rclcpp::Publisher<std_msgs::msg::String>> mypubs;
};

#endif // ROS_PLUGIN_HPP
