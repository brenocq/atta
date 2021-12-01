cmake_minimum_required(VERSION 3.12)

set(ATTA_PHYSICS_SYSTEM_SOURCE
    physicsManager.cpp
	
	physicsEngines/physicsEngine.cpp
	physicsEngines/nullEngine.cpp
	physicsEngines/box2DEngine.cpp
)

add_library(atta_physics_system STATIC
    ${ATTA_PHYSICS_SYSTEM_SOURCE}
)
atta_target_common(atta_physics_system)
atta_add_libs(atta_physics_system)
target_link_libraries(atta_physics_system PUBLIC box2d)
