cmake_minimum_required(VERSION 3.12)

########## Algorithms ##########
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/linearAlgebra)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/computerVision)

set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES} PARENT_SCOPE)
set(ATTA_LIBS ${ATTA_LIBS} PARENT_SCOPE)
