cmake_minimum_required(VERSION 3.12)

set(ATTA_COMPONENT_SYSTEM_SOURCE
	componentManager.cpp
)

add_library(atta_component_system_lib STATIC
	${ATTA_COMPONENT_SYSTEM_SOURCE}
)
atta_target_common(atta_component_system_lib)
atta_add_libs(atta_component_system_lib)
