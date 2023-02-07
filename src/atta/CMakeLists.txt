cmake_minimum_required(VERSION 3.14)

set(ATTA_SOURCE
    atta.cpp
)

add_library(atta_lib STATIC
    ${ATTA_SOURCE}
)
atta_target_common(atta_lib)
atta_add_libs(atta_lib)

########## Subsystems ##########
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/memory)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/event)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/file)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/component)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/graphics)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resource)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/script)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/parallel)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/physics)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sensor)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/io)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ui)

set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES} PARENT_SCOPE)
set(ATTA_LIBS ${ATTA_LIBS} PARENT_SCOPE)
