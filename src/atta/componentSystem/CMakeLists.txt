cmake_minimum_required(VERSION 3.12)

set(ATTA_COMPONENT_SYSTEM_SOURCE
	entity.cpp
	componentManager.cpp
)

add_library(atta_component_system STATIC
	${ATTA_COMPONENT_SYSTEM_SOURCE}
)
atta_target_common(atta_component_system)
atta_add_libs(atta_component_system)

# Public headers
set(ATTA_COMPONENT_SYSTEM_PUBLIC_HEADERS
	componentManager.h
	components/components.h
	components/meshComponent.h
	components/nameComponent.h
	components/transformComponent.h
)
#set_target_properties(atta_component_system PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/componentSystem/componentManager.h")
atta_install(FILES ${ATTA_COMPONENT_SYSTEM_PUBLIC_HEADERS} DESTINATION componentManager)
