cmake_minimum_required(VERSION 3.14)

set(ATTA_PARALLEL_MODULE_SOURCE
    interface.cpp
    manager.cpp

    devices/device.cpp
    devices/serialDevice.cpp
    devices/cpuDevice.cpp
)

add_library(atta_parallel_module STATIC
    ${ATTA_PARALLEL_MODULE_SOURCE}
)
atta_target_common(atta_parallel_module)
atta_add_libs(atta_parallel_module)
