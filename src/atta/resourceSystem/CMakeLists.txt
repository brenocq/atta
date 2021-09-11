cmake_minimum_required(VERSION 3.12)

set(ATTA_RESOURCE_SYSTEM_SOURCE
	resourceManager.cpp
	resource.cpp

	resources/mesh.cpp
)

add_library(atta_resource_system STATIC
	${ATTA_RESOURCE_SYSTEM_SOURCE}
)
atta_target_common(atta_resource_system)
atta_add_libs(atta_resource_system)
target_link_libraries(atta_resource_system PRIVATE atta_memory_system assimp)
