cmake_minimum_required(VERSION 3.14)

set(ATTA_SENSOR_MODULE_SOURCE
    interface.cpp
    manager.cpp
)

add_library(atta_sensor_module STATIC
    ${ATTA_SENSOR_MODULE_SOURCE}
)

target_link_libraries(atta_sensor_module PRIVATE atta_graphics_module)
atta_target_common(atta_sensor_module)
atta_add_libs(atta_sensor_module)
