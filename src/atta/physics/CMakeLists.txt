cmake_minimum_required(VERSION 3.14)

set(ATTA_PHYSICS_MODULE_SOURCE
    interface.cpp
    manager.cpp
	
	engines/engine.cpp
	engines/noneEngine.cpp
	engines/box2DEngine.cpp
	engines/bulletEngine.cpp
)

add_library(atta_physics_module STATIC
    ${ATTA_PHYSICS_MODULE_SOURCE}
)
atta_target_common(atta_physics_module)
atta_add_libs(atta_physics_module)
target_link_libraries(atta_physics_module PUBLIC ${ATTA_BOX2D_TARGETS} ${ATTA_BULLET_TARGETS})
