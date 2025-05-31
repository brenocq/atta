cmake_minimum_required(VERSION 3.14)
project(ros_plugin)

if(WITH_ROS2)
    # Find ROS2 dependencies
    set(AMENT_CMAKE_UNINSTALL_TARGET OFF)
    find_package(ament_cmake REQUIRED)
    find_package(rclcpp REQUIRED)
    find_package(std_msgs REQUIRED)
    find_package(std_srvs REQUIRED)
    find_package(tf2_ros REQUIRED)
    find_package(geometry_msgs REQUIRED)

    set(ROS_PLUGIN_SOURCE   src/RosPlugin.cpp)
    set(ROS_PLUGIN_HEADERS  include/RosPlugin.hpp include/Util.hpp)

    
else()
    # use the stub instead   
    message(STATUS "Building without ROS2 support, using stub.")
    set(ROS_PLUGIN_SOURCE src/RosPluginStub.cpp)
    set(ROS_PLUGIN_HEADERS include/RosPluginStub.hpp)


endif()


# Create a static library
add_library(ros_plugin STATIC
    ${ROS_PLUGIN_SOURCE}
    ${ROS_PLUGIN_HEADERS}
)

if(WITH_ROS2)
    # Link the ROS2 libraries
    target_include_directories(ros_plugin PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            ${rclcpp_INCLUDE_DIRS}
                            ${std_msgs_INCLUDE_DIRS}
                            ${std_srvs_INCLUDE_DIRS}
                            ${geometry_msgs_INCLUDE_DIRS})
    ament_target_dependencies(ros_plugin
                            rclcpp
                            std_msgs
                            std_srvs
                            tf2_ros
                            geometry_msgs
                        )
                        
else()
    # Link the ROS2 stub libraries
    target_include_directories(ros_plugin PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            )
endif()



atta_target_common(ros_plugin)
atta_add_libs(ros_plugin)


install(
    TARGETS ros_plugin
    EXPORT ros_plugin_export
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

if(WITH_ROS2)
    ament_export_dependencies(rclcpp std_msgs std_srvs geometry_msgs)
    ament_export_targets(ros_plugin_export HAS_LIBRARY_TARGET)
    ament_package()
endif()

if(TARGET atta_lib)
    target_link_libraries(atta_lib PUBLIC ros_plugin)
endif()