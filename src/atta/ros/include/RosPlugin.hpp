#ifndef ROS_PLUGIN_HPP
#define ROS_PLUGIN_HPP

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <thread>
#include <memory>
#include <atta/utils/config.h>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <atta/event/event.h>
#include <atta/event/events/createComponent.h>
#include <atta/component/components/component.h>
#include <atta/component/components/transform.h>
#include <geometry_msgs/msg/pose.hpp>


class rosPlugin {
public:
    rosPlugin();
    ~rosPlugin();

    void update();
    void publishData(std::string msg);

    //create transform Publisher
    void createTransformPublisher(const atta::event::CreateComponent& event);
    //___
    bool deleteTransformPub(int id);
private:
    void createPublishers();
    void createServices();

    void deletePublisher();
    void createThread();
    bool rosInitializedHere = false;
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor_;
    std::thread executor_thread_;
    std::vector<rclcpp::Publisher<std_msgs::msg::String>> mypubs;
    //Maps of Publisher
    std::unordered_map<std::string, rclcpp::Publisher<std_msgs::msg::String>::SharedPtr> stringPubs;
    std::unordered_map<int, rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr> transformPubs;
    void deleteAllPubs();
    //___

    //topic update
    void updateTransform();
    //___

    //standard services
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr pausePhysics;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr unPausePhysics;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr resetSimulation;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr stepSimulation;
    void pauseCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);
    void unPauseCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);      
    void resetCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);               
    void stepCallback(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                             std::shared_ptr<std_srvs::srv::Trigger::Response> response);                                
    //___
};

#endif // ROS_PLUGIN_HPP
