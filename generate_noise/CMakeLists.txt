cmake_minimum_required(VERSION 3.0.2)
project(generate_noise)

find_package(catkin REQUIRED roscpp sensor_msgs nav_msgs std_msgs)

catkin_package()

include_directories(
${catkin_INCLUDE_DIRS}
)

add_executable(generate_noise src/generate_noise.cpp)
target_link_libraries(generate_noise ${catkin_LIBRARIES} )