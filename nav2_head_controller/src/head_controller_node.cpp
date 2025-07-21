#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>
#include <controller_manager_msgs/srv/list_controllers.hpp>
#include <action_msgs/msg/goal_status_array.hpp>

using std::placeholders::_1;

class HeadControllerNode : public rclcpp::Node
{
public:
    HeadControllerNode()
    : Node("nav2_head_controller"),
      goal_pan_(0.0), goal_tilt_(0.0), is_new_data_(false), moving_time(1.5)
    {
        pub_hsr_head_gp_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>(
            "/head_trajectory_controller/joint_trajectory", 10);
        pub_current_pose_ = this->create_publisher<std_msgs::msg::Float64MultiArray>(
            "/hardware/head/current_pose", 10);
        sub_goal_pose_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
            "/hardware/head/goal_pose", 10, std::bind(&HeadControllerNode::headGoalPoseCallback, this, _1));
        sub_nav_status_ = this->create_subscription<action_msgs::msg::GoalStatusArray>(
            "/navigate_to_pose/_action/status", 10,
            std::bind(&HeadControllerNode::navStatusCallback, this, std::placeholders::_1));
        client_list_controllers_ = this->create_client<controller_manager_msgs::srv::ListControllers>(
            "/controller_manager/list_controllers");
        traj_.joint_names = {"head_pan_joint", "head_tilt_joint"};
        traj_.points.resize(1);
        traj_.points[0].positions.resize(2, 0.0);
        traj_.points[0].velocities.resize(2, 0.1);
        traj_.points[0].time_from_start = rclcpp::Duration::from_seconds(0.05);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(33), std::bind(&HeadControllerNode::controlLoop, this));
        waitForController();
    }

private:
    void headGoalPoseCallback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
    {
        goal_pan_ = std::clamp(msg->data[0], -3.141592, 1.74);
        goal_tilt_ = std::clamp(msg->data[1], -0.9, 0.47);
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
        goal_tilt_ = (has_executing) ? -0.9 : 0.0;
        traj_.points[0].positions[0] = goal_pan_;
        traj_.points[0].positions[1] = goal_tilt_;
        is_new_data_ = true;
        traj_.points[0].time_from_start = rclcpp::Duration::from_seconds(moving_time);
        pub_hsr_head_gp_->publish(traj_);
        sleep(moving_time);
        if (has_executing) moving_time = 0.5;
    }

    void waitForController()
    {
        RCLCPP_INFO(this->get_logger(), "Waiting for head_trajectory_controller to become active...");
        while (!client_list_controllers_->wait_for_service(std::chrono::seconds(1))) {
            RCLCPP_WARN(this->get_logger(), "Waiting for /controller_manager/list_controllers service...");
        }
        while (rclcpp::ok()) {
            auto request = std::make_shared<controller_manager_msgs::srv::ListControllers::Request>();
            auto result_future = client_list_controllers_->async_send_request(request);
            if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result_future) ==
                rclcpp::FutureReturnCode::SUCCESS)
            {
                auto result = result_future.get();
                for (const auto &controller : result->controller) {
                    if (controller.name == "head_trajectory_controller" && controller.state == "active") {
                        RCLCPP_INFO(this->get_logger(), "head_trajectory_controller is active.");
                        return;
                    }
                }
            }
            rclcpp::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void controlLoop()
    {
        if (moving_time < 1.0)
        {
            traj_.points[0].positions[0] = goal_pan_;
            traj_.points[0].positions[1] = goal_tilt_;
            traj_.points[0].time_from_start = rclcpp::Duration::from_seconds(moving_time);
            if (is_new_data_) {
                pub_hsr_head_gp_->publish(traj_);
                is_new_data_ = false;
            }
        }
    }
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_hsr_head_gp_;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr pub_current_pose_;
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr sub_goal_pose_;
    rclcpp::Subscription<action_msgs::msg::GoalStatusArray>::SharedPtr sub_nav_status_;
    rclcpp::Client<controller_manager_msgs::srv::ListControllers>::SharedPtr client_list_controllers_;
    trajectory_msgs::msg::JointTrajectory traj_;
    float goal_pan_, goal_tilt_;
    bool is_new_data_;
    float moving_time;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HeadControllerNode>());
    rclcpp::shutdown();
    return 0;
}
