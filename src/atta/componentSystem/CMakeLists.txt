cmake_minimum_required(VERSION 3.12)

set(ATTA_COMPONENT_SYSTEM_SOURCE
    entity.cpp
    factory.cpp
    componentManager.cpp

    components/component.cpp
    components/cameraComponent.cpp
    components/directionalLightComponent.cpp
    components/materialComponent.cpp
    components/meshComponent.cpp
    components/nameComponent.cpp
    components/pointLightComponent.cpp
    components/prototypeComponent.cpp
    components/relationshipComponent.cpp
    components/scriptComponent.cpp
    components/transformComponent.cpp
)

add_library(atta_component_system STATIC
    ${ATTA_COMPONENT_SYSTEM_SOURCE}
)

atta_target_common(atta_component_system)
atta_add_libs(atta_component_system)
