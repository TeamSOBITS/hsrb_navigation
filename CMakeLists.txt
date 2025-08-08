cmake_minimum_required(VERSION 3.8)
project(flex_nav)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(trajectory_msgs REQUIRED)
find_package(action_msgs REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(head_angle_publisher_node src/head_angle_publisher_node.cpp)
ament_target_dependencies(head_angle_publisher_node
  rclcpp
  std_msgs
  nav_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
)
ament_target_dependencies(head_angle_publisher_node yaml-cpp)

add_executable(head_controller_node src/head_controller_node.cpp)
ament_target_dependencies(head_controller_node
  rclcpp
  std_msgs
  trajectory_msgs
  action_msgs
)
ament_target_dependencies(head_controller_node yaml-cpp)

install(TARGETS
  head_angle_publisher_node
  head_controller_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY
  param
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()