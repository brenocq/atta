cmake_minimum_required(VERSION 3.14)

set(ATTA_PARALLEL_MODULE_SOURCE
    interface.cpp
    manager.cpp

    devices/device.cpp
    devices/serialDevice.cpp
    devices/cpuDevice.cpp
    devices/gpuDevice.cpp
    devices/add_vectors.cu
)

add_library(atta_parallel_module STATIC ${ATTA_PARALLEL_MODULE_SOURCE})
if(${ATTA_CUDA_SUPPORT})
    target_link_libraries(atta_parallel_module PRIVATE ${ATTA_CUDA_TARGETS})
endif()

atta_target_common(atta_parallel_module)
atta_add_libs(atta_parallel_module)
