#ifndef ROS_PLUGIN_HPP
#define ROS_PLUGIN_HPP

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <std_msgs/msg/string.hpp>
#include <thread>
#include <memory>
#include <atta/utils/config.h>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <sensor_msgs/msg/range.hpp>
#include <rosgraph_msgs/msg/clock.hpp>
#include "ros_plugin/msg/rigid_body.hpp"
#include <atta/event/event.h>
#include <atta/event/events/createComponent.h>
#include <atta/component/components/component.h>
namespace atta::ros {

class rosPlugin {
public:
    rosPlugin();
    ~rosPlugin();

    void update();
    void publishData(std::string msg);

    //create/delete transform Topics
    void createTransformTopics(const atta::event::CreateComponent& event);
    void deleteTransformTopics(int id);
    //create/delete IR Topics
    void createIRTopics(const atta::event::CreateComponent& event);
    void deleteIRTopics(int id);
    //create/delete RigidBody topics
    void createRigidTopics(const atta::event::CreateComponent& event);
    void deleteRigidTopics(int id);
    //___
    void deleteAllTopics();
private:
    //setup
    bool rosInitializedHere = false;
    rclcpp::Node::SharedPtr node_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor_;
    std::thread executor_thread_;
    //___
    //Maps of Topics
    std::unordered_map<std::string, rclcpp::Publisher<std_msgs::msg::String>::SharedPtr> stringPubs;
    std::unordered_map<int, rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr> transformPubs;
    std::unordered_map<int, rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr> transformSubs;
    std::unordered_map<int, rclcpp::Publisher<sensor_msgs::msg::Range>::SharedPtr> IRPubs;
    std::unordered_map<int, rclcpp::Publisher<ros_plugin::msg::RigidBody>::SharedPtr> RigidBodyPubs;
    std::unordered_map<int, rclcpp::Subscription<ros_plugin::msg::RigidBody>::SharedPtr> RigidBodySubs;
    //___
    // SubscribersCallBack
    void transformCallback(const geometry_msgs::msg::Pose::SharedPtr msg, int key) const;
    void rigidBodyCallback(const ros_plugin::msg::RigidBody::SharedPtr msg, int key) const;
    //___
    //topic update
    void updateTransform(); // update the trasform componeents x,y,z and orientation r,i,j,k
    void updateIr();        // update the IR components with the IR Distance
    void updateClock();     // update the Ros CLock
    void updateCamera();    // update the Camera Feedback
    void updateRigidBody(); // update the RigidBody physics data
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
    void createServices();
    void deleteServices();
    //___

    void createPublishers();
    

    void deletePublisher();
    void createThread();
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Publisher<rosgraph_msgs::msg::Clock>::SharedPtr rosClock;
    //rclcpp::TimerBase::SharedPtr timer_;
};
}
#endif // ROS_PLUGIN_HPP
