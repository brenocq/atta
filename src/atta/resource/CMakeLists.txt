cmake_minimum_required(VERSION 3.14)

set(ATTA_RESOURCE_MODULE_SOURCE
    interface.cpp
    manager.cpp

    resource.cpp
    resources/mesh.cpp
    resources/image.cpp
    resources/material.cpp
)

add_library(atta_resource_module STATIC
    ${ATTA_RESOURCE_MODULE_SOURCE}
)
atta_target_common(atta_resource_module)
atta_add_libs(atta_resource_module)
target_link_libraries(atta_resource_module PUBLIC atta_memory_module ${ATTA_ASSIMP_TARGETS})
