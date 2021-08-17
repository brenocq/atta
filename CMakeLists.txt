cmake_minimum_required(VERSION 3.12)

project(Atta VERSION 0.0.2.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

########## Build Types ##########
if(CMAKE_BUILD_TYPE MATCHES Debug)
	list(APPEND ATTA_DEFINITIONS "ATTA_DEBUG_BUILD")
endif()

########## OS ##########
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
	list(APPEND ATTA_DEFINITIONS "ATTA_IS_WINDOWS")
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
	list(APPEND ATTA_DEFINITIONS "ATTA_IS_MACOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	list(APPEND ATTA_DEFINITIONS "ATTA_IS_LINUX")
else()
	message(SEND_ERROR "Unknown system name: " + CMAKE_SYSTEM_NAME)
endif()

########## Output Directories ##########
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)
set(TESTS_OUTPUT_DIR ${EXECUTABLE_OUTPUT_PATH}/tests CACHE INTERNAL "" FORCE)
set(ATTA_PATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "" FORCE)

########## Precompiled Headers ##########
list(APPEND ATTA_PCH "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/pch.h")

########## Extern ##########
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/extern)

########## Testing ##########
include(GoogleTest)
enable_testing()

########## Subsystems ##########
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/atta/eventSystem)

########## Extern ##########
add_executable(atta "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/core/log.cpp")
target_compile_definitions(atta PRIVATE ${ATTA_DEFINITIONS})
target_precompile_headers(atta PRIVATE ${ATTA_PCH})
target_include_directories(atta PUBLIC
	${ATTA_PATH}/src
)
target_compile_definitions(atta PRIVATE ${ATTA_DEFINITIONS})
