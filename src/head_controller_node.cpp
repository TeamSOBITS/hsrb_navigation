#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <action_msgs/msg/goal_status_array.hpp>
#include <cmath>
#include <string> 

using std::placeholders::_1;

class HeadControllerNode : public rclcpp::Node
{
public:
    HeadControllerNode()
    : Node("head_controller_node"), is_new_data_(false), moving_time(1.5)
    {
        this->declare_parameter("head_trajectory_topic", "/hsrb/head_trajectory_controller/command");
        this->declare_parameter("head_pan_joint_name", "head_pan_joint");
        this->declare_parameter("head_tilt_joint_name", "head_tilt_joint");
        this->declare_parameter("max_pan_limit", 1.74);
        this->declare_parameter("min_pan_limit", -3.141592);
        this->declare_parameter("max_tilt_limit", 0.47);
        this->declare_parameter("min_tilt_limit", -0.9);
        this->declare_parameter("tilt_angle_navigating", -0.9);
        this->declare_parameter("head_rotation_type", "pan_tilt");

        head_trajectory_topic_ = this->get_parameter("head_trajectory_topic").as_string();
        head_pan_joint_ = this->get_parameter("head_pan_joint_name").as_string();
        head_tilt_joint_ = this->get_parameter("head_tilt_joint_name").as_string();
        max_pan_limit_ = this->get_parameter("max_pan_limit").as_double();
        min_pan_limit_ = this->get_parameter("min_pan_limit").as_double();
        max_tilt_limit_ = this->get_parameter("max_tilt_limit").as_double();
        min_tilt_limit_ = this->get_parameter("min_tilt_limit").as_double();
        tilt_angle_navigating_ = this->get_parameter("tilt_angle_navigating").as_double();
        head_rotation_type_ = this->get_parameter("head_rotation_type").as_string();

        if ((head_rotation_type_ != "pan_tilt") && (head_rotation_type_ != "rpy")) return;

        RCLCPP_INFO(this->get_logger(), "head_trajectory_topic: %s", head_trajectory_topic_.c_str());
        RCLCPP_INFO(this->get_logger(), "head_pan_joint: %s", head_pan_joint_.c_str());
        RCLCPP_INFO(this->get_logger(), "head_tilt_joint: %s", head_tilt_joint_.c_str());
        RCLCPP_INFO(this->get_logger(), "max_pan_limit: %f", max_pan_limit_);
        RCLCPP_INFO(this->get_logger(), "min_pan_limit: %f", min_pan_limit_);
        RCLCPP_INFO(this->get_logger(), "max_tilt_limit: %f", max_tilt_limit_);
        RCLCPP_INFO(this->get_logger(), "min_tilt_limit: %f", min_tilt_limit_);
        RCLCPP_INFO(this->get_logger(), "tilt_angle_navigating: %f", tilt_angle_navigating_);
        RCLCPP_INFO(this->get_logger(), "head_rotation_type: %s", head_rotation_type_.c_str());

        pub_head_gp_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>(
            head_trajectory_topic_, 1);

        sub_goal_pose_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
            "/hardware/head/goal_pose", 10, std::bind(&HeadControllerNode::headGoalPoseCallback, this, _1));
        sub_nav_status_ = this->create_subscription<action_msgs::msg::GoalStatusArray>(
            "/navigate_to_pose/_action/status", 10,
            std::bind(&HeadControllerNode::navStatusCallback, this, std::placeholders::_1));

        traj_.joint_names = {head_pan_joint_, head_tilt_joint_};
        traj_.points.resize(1);
        traj_.points[0].positions.resize(2, 0.0);
        traj_.points[0].velocities.resize(2, 0.1);
        traj_.points[0].time_from_start = rclcpp::Duration::from_seconds(0.05);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(33), std::bind(&HeadControllerNode::controlLoop, this));
    }

private:
    void headGoalPoseCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
    {
        double tilt_temp = (head_rotation_type_ == "pan_tilt") ? msg->data[1] : -msg->data[1];
        goal_pan_ = std::clamp(msg->data[0], min_pan_limit_, max_pan_limit_);
        goal_tilt_ = std::clamp(tilt_temp, min_tilt_limit_, max_tilt_limit_);
        is_new_data_ = true;
    }

    void navStatusCallback(const action_msgs::msg::GoalStatusArray::SharedPtr msg)
    {
        bool has_executing = false;
        for (const auto & status : msg->status_list) {
            if (status.status == action_msgs::msg::GoalStatus::STATUS_EXECUTING) {
                has_executing = true;
                break;
            }
        }
        
        moving_time = 1.5;
        goal_pan_ = 0.0;
        goal_tilt_ = (has_executing) ? tilt_angle_navigating_ : 0.0;
        
        traj_.points[0].positions[0] = goal_pan_;
        traj_.points[0].positions[1] = goal_tilt_;
        is_new_data_ = true;
        traj_.points[0].time_from_start = rclcpp::Duration::from_seconds(moving_time);
        
        pub_head_gp_->publish(traj_);

        rclcpp::sleep_for(std::chrono::seconds(static_cast<int>(moving_time)));
        
        if (has_executing) moving_time = 0.5;
    }

    void controlLoop()
    {
        if (moving_time < 1.0)
        {
            traj_.points[0].positions[0] = goal_pan_;
            traj_.points[0].positions[1] = goal_tilt_;
            traj_.points[0].time_from_start = rclcpp::Duration::from_seconds(moving_time);

            if (is_new_data_) {
                pub_head_gp_->publish(traj_);
                is_new_data_ = false;
            }
        }
    }

    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_head_gp_;
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr sub_goal_pose_;
    rclcpp::Subscription<action_msgs::msg::GoalStatusArray>::SharedPtr sub_nav_status_;
    trajectory_msgs::msg::JointTrajectory traj_;
    float goal_pan_, goal_tilt_;
    bool is_new_data_;
    float moving_time;
    rclcpp::TimerBase::SharedPtr timer_;
    
    std::string head_trajectory_topic_;
    std::string head_pan_joint_; 
    std::string head_tilt_joint_; 
    double max_pan_limit_;
    double min_pan_limit_;
    double max_tilt_limit_;
    double min_tilt_limit_;
    double tilt_angle_navigating_;
    std::string head_rotation_type_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    
    rclcpp::spin(std::make_shared<HeadControllerNode>());
    
    rclcpp::shutdown();
    
    return 0;
}