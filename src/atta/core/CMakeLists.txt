cmake_minimum_required(VERSION 3.12)

set(ATTA_CORE_SOURCE
	log.cpp
	stringId.cpp
	window.cpp
)

add_library(atta_core_lib STATIC
	${ATTA_CORE_SOURCE}
)
atta_target_common(atta_core_lib)
atta_add_libs(atta_core_lib)
target_link_libraries(atta_core_lib PUBLIC glfw)

########## Testing ##########
atta_add_tests("tests/stringId.cpp")
