cmake_minimum_required(VERSION 3.14)

set(ATTA_COMPONENT_MODULE_SOURCE
    interface.cpp
    manager.cpp

    entity.cpp
    factory.cpp

    registry/registry.cpp
    registry/typedRegistry.cpp

    dataManager/cpuDataManager.cpp
    dataManager/dataManager.cpp
    dataManager/gpuDataManager.cpp

    components/boxCollider.cpp
    components/boxCollider2D.cpp
    components/cameraSensor.cpp
    components/circleCollider2D.cpp
    components/cylinderCollider.cpp
    components/directionalLight.cpp
    components/environmentLight.cpp
    components/infraredSensor.cpp
    components/material.cpp
    components/mesh.cpp
    components/name.cpp
    components/pointLight.cpp
    components/prismaticJoint.cpp
    components/prototype.cpp
    components/relationship.cpp
    components/revoluteJoint.cpp
	components/rigidBody.cpp
	components/rigidBody2D.cpp
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
