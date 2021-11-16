cmake_minimum_required(VERSION 3.12)

set(ATTA_COMPONENT_SYSTEM_SOURCE
    entity.cpp
    factory.cpp
    componentManager.cpp
    componentRegistry.cpp

    components/meshComponent.cpp
    components/relationshipComponent.cpp
    components/transformComponent.cpp
)

add_library(atta_component_system STATIC
    ${ATTA_COMPONENT_SYSTEM_SOURCE}
)

atta_target_common(atta_component_system)
atta_add_libs(atta_component_system)
