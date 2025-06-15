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
    find_package(sensor_msgs)
    find_package(rosidl_default_generators REQUIRED)
    
    set(ROS_PLUGIN_SOURCE   
         src/RosPlugin.cpp
         src/interface.cpp)
    set(ROS_PLUGIN_HEADERS  
         include/RosPlugin.hpp
         include/Util.hpp
         include/interface.hpp)

    rosidl_generate_interfaces(${PROJECT_NAME}
        "msg/RigidBody.msg"
        DEPENDENCIES std_msgs geometry_msgs
    )

    
else()
    # use the stub instead   
    message(STATUS "Building without ROS2 support, using stub.")
    set(ROS_PLUGIN_SOURCE 
         src/RosPluginStub.cpp
         src/interface.cpp)
    set(ROS_PLUGIN_HEADERS 
         include/RosPluginStub.hpp
         include/interface.hpp)


endif()


# Create a static library
add_library(ros_plugin_lib STATIC
    ${ROS_PLUGIN_SOURCE}
    ${ROS_PLUGIN_HEADERS}
)

if(WITH_ROS2)
    # Link the ROS2 libraries
    rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
    target_link_libraries(ros_plugin_lib "${cpp_typesupport_target}")
    target_include_directories(ros_plugin_lib PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            ${rclcpp_INCLUDE_DIRS}
                            #${std_msgs_INCLUDE_DIRS}
                            #${std_srvs_INCLUDE_DIRS}
                            #${geometry_msgs_INCLUDE_DIRS}
                            #${sensor_msgs_INCLUDE_DIRS}
                            )   
    ament_target_dependencies(ros_plugin_lib
                            rclcpp
                            std_msgs
                            std_srvs
                            tf2_ros
                            geometry_msgs
                            sensor_msgs
                            rosgraph_msgs
                            rosidl_default_runtime
                            )
                        
else()
    # Link the ROS2 stub libraries
    target_include_directories(ros_plugin_lib PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            )
endif()



atta_target_common(ros_plugin_lib)
atta_add_libs(ros_plugin_lib)


install(
    TARGETS ros_plugin_lib
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
    target_link_libraries(atta_lib PUBLIC ros_plugin_lib)
endif()