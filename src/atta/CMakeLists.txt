cmake_minimum_required(VERSION 3.12)

set(ATTA_SOURCE
	atta.cpp
)

add_library(atta_lib STATIC
	${ATTA_SOURCE}
)
atta_target_common(atta_lib)
atta_add_libs(atta_lib)

########## Subsystems ##########
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/memorySystem)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/eventSystem)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fileSystem)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/componentSystem)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/graphicsSystem)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/serviceLocator)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/algorithms)

set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES} PARENT_SCOPE)
set(ATTA_LIBS ${ATTA_LIBS} PARENT_SCOPE)
