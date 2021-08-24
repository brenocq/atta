cmake_minimum_required(VERSION 3.12)

set(GLAD_SOURCE
	src/glad.c
)

add_library(glad STATIC
	${GLAD_SOURCE}
)
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
