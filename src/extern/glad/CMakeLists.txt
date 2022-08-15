cmake_minimum_required(VERSION 3.14)

set(GLAD_SOURCE
	src/glad.c
)

add_library(glad STATIC
	${GLAD_SOURCE}
)
target_include_directories(glad PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/glad/include>
)
