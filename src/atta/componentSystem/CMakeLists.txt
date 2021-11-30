cmake_minimum_required(VERSION 3.12)

set(ATTA_COMPONENT_SYSTEM_SOURCE
    entity.cpp
    factory.cpp
    componentManager.cpp
    componentRegistry.cpp

    components/boxCollider2DComponent.cpp
    components/cameraComponent.cpp
    components/circleCollider2DComponent.cpp
    components/directionalLightComponent.cpp
    components/materialComponent.cpp
    components/meshComponent.cpp
    components/nameComponent.cpp
    components/pointLightComponent.cpp
    components/prototypeComponent.cpp
    components/relationshipComponent.cpp
	components/rigidBody2DComponent.cpp
    components/scriptComponent.cpp
    components/transformComponent.cpp
)

add_library(atta_component_system STATIC
    ${ATTA_COMPONENT_SYSTEM_SOURCE}
)
target_link_libraries(atta_component_system PUBLIC imgui)

atta_target_common(atta_component_system)
atta_add_libs(atta_component_system)
