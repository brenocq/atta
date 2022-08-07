cmake_minimum_required(VERSION 3.12)

set(ATTA_COMPONENT_MODULE_SOURCE
    entity.cpp
    factory.cpp
    componentManager.cpp
    componentRegistry.cpp
    typedComponentRegistry.cpp

    components/boxCollider.cpp
    components/camera.cpp
    components/directionalLight.cpp
    components/environmentLight.cpp
    components/material.cpp
    components/mesh.cpp
    components/name.cpp
    components/pointLight.cpp
    components/prismaticJoint.cpp
    components/prototype.cpp
    components/relationship.cpp
    components/revoluteJoint.cpp
	components/rigidBody2D.cpp
	components/rigidBody.cpp
	components/rigidJoint.cpp
    components/script.cpp
    components/sphereCollider.cpp
    components/transform.cpp
)

add_library(atta_component_module STATIC ${ATTA_COMPONENT_MODULE_SOURCE})
target_link_libraries(atta_component_module PUBLIC ${ATTA_IMGUI_TARGETS} 
    atta_physics_module atta_memory_module atta_sensor_module atta_script_module)

atta_target_common(atta_component_module)
atta_add_libs(atta_component_module)
