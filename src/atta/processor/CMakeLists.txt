cmake_minimum_required(VERSION 3.14)

set(ATTA_PROCESSOR_MODULE_SOURCE
    interface.cpp
    manager.cpp

    processors/processor.cpp
    processors/serial.cpp
    processors/cpu.cpp
    processors/gpu.cpp
)

add_library(atta_processor_module STATIC ${ATTA_PROCESSOR_MODULE_SOURCE})
target_link_libraries(atta_processor_module PUBLIC atta_component_module atta_script_module)

atta_target_common(atta_processor_module)
atta_add_libs(atta_processor_module)

if(ATTA_CUDA_SUPPORT)
    atta_cuda_sources(atta_processor_module "processors/gpu.cpp")
endif()
