cmake_minimum_required(VERSION 3.10)

project(Atta VERSION 0.0.2.0 LANGUAGES CXX)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DATTA_DEBUG_BUILD)
endif()

add_executable(atta "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/core/log.cpp")
target_precompile_headers(atta PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/pch.h")
