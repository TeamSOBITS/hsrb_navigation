// head_angle_publisher_node.cpp
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/path.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/utils.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <cmath>

class HeadAnglePublisherNode : public rclcpp::Node
{
public:
    HeadAnglePublisherNode() : Node("head_angle_publisher_node"), tf_buffer_(this->get_clock()), tf_listener_(tf_buffer_)
    {
        using std::placeholders::_1;
        this->declare_parameter("path_topic", "/plan");
        this->declare_parameter("head_goal_topic", "/hardware/head/goal_pose");
        this->declare_parameter("base_link", "base_footprint");
        this->declare_parameter("look_ahead_index", 5);
        path_topic_ = this->get_parameter("path_topic").as_string();
        head_goal_topic_ = this->get_parameter("head_goal_topic").as_string();
        base_link_ = this->get_parameter("base_link").as_string();
        look_ahead_index_ = this->get_parameter("look_ahead_index").as_int();
        head_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>(head_goal_topic_, 10);
        path_sub_ = this->create_subscription<nav_msgs::msg::Path>(
            path_topic_, 10, std::bind(&HeadAnglePublisherNode::path_callback, this, _1));

        RCLCPP_INFO(this->get_logger(), "Head angle publisher node started.");
    }

private:
    void path_callback(const nav_msgs::msg::Path::SharedPtr msg)
    {
        if (msg->poses.empty()) return;
        geometry_msgs::msg::TransformStamped tf;
        try {
            tf = tf_buffer_.lookupTransform("map", base_link_, tf2::TimePointZero);
        } catch (tf2::TransformException & ex) {
            RCLCPP_WARN(this->get_logger(), "Could not transform map to %s: %s", base_link_.c_str(), ex.what());
            return;
        }
        geometry_msgs::msg::Quaternion q_msg = tf.transform.rotation;
        tf2::Quaternion q_tf;
        tf2::fromMsg(q_msg, q_tf);
        double robot_x = tf.transform.translation.x;
        double robot_y = tf.transform.translation.y;
        double robot_yaw = tf2::getYaw(q_tf);
        int idx = std::min((size_t)look_ahead_index_, msg->poses.size() - 1);
        double goal_x = msg->poses[idx].pose.position.x;
        double goal_y = msg->poses[idx].pose.position.y;
        double angle = std::atan2(goal_y - robot_y, goal_x - robot_x) - robot_yaw;
        angle = std::atan2(std::sin(angle), std::cos(angle)); // normalize [-pi, pi]
        std_msgs::msg::Float64MultiArray head_cmd;
        head_cmd.data.push_back(angle);
        head_cmd.data.push_back(-1.0);  // tilt angle fixed
        head_pub_->publish(head_cmd);
    }
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr head_pub_;
    rclcpp::Subscription<nav_msgs::msg::Path>::SharedPtr path_sub_;
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;
    std::string path_topic_;
    std::string head_goal_topic_;
    std::string base_link_;
    int look_ahead_index_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HeadAnglePublisherNode>());
    rclcpp::shutdown();
    return 0;
}
