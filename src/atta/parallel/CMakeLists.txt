cmake_minimum_required(VERSION 3.14)

set(ATTA_PARALLEL_MODULE_SOURCE
    interface.cpp
    manager.cpp

    devices/device.cpp
    devices/serialDevice.cpp
    devices/cpuDevice.cpp
    devices/gpuDevice.cpp
)

add_library(atta_parallel_module STATIC ${ATTA_PARALLEL_MODULE_SOURCE})
target_link_libraries(atta_parallel_module PUBLIC atta_component_module atta_script_module)

atta_target_common(atta_parallel_module)
atta_add_libs(atta_parallel_module)

#if(ATTA_CUDA_SUPPORT)
#    atta_cuda_sources(atta_parallel_module "devices/gpuDevice.cpp")
#endif()
