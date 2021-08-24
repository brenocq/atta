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
	# Force google test to link the runtimes dynamically (to avoid visual studio link error)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
	list(APPEND ATTA_DEFINITIONS "ATTA_IS_MACOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	list(APPEND ATTA_DEFINITIONS "ATTA_IS_LINUX")
else()
	message(SEND_ERROR "Unknown system name: " + CMAKE_SYSTEM_NAME)
endif()

########## Options ##########
if(CMAKE_BUILD_TYPE MATCHES Debug)
	if(MSVC)
		list(APPEND ATTA_OPTIONS /W4 /WX)
	else()
		list(APPEND ATTA_OPTIONS -Wall -Wextra -Wpedantic -Werror)
		# Unused parameter
		list(APPEND ATTA_OPTIONS -Wno-unused-parameter)
		# Math anonymous union 
		list(APPEND ATTA_OPTIONS -Wno-nested-anon-types -Wno-gnu-anonymous-struct)
		# Glad opengl definitions
		list(APPEND ATTA_OPTIONS -Wno-macro-redefined)
	endif()
endif()

########## Output Directories ##########
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)
set(ATTA_PATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "" FORCE)

########## Precompiled Headers ##########
list(APPEND ATTA_PCH "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/pch.h")

########## Macros ##########
macro(atta_add_libs)
	foreach(_lib ${ARGN})
		list(APPEND ATTA_LIBS ${_lib})
	endforeach()
	set(ATTA_LIBS ${ATTA_LIBS} PARENT_SCOPE)
endmacro()

macro(atta_add_include_dirs)
	foreach(_include_dir ${ARGN})
		list(APPEND ATTA_INCLUDE_DIRS ${_include_dir})
	endforeach()
	set(ATTA_INCLUDE_DIRS ${ATTA_INCLUDE_DIRS} PARENT_SCOPE)
endmacro()

macro(atta_add_tests)
	foreach(_test ${ARGN})
		list(APPEND ATTA_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${_test}")
	endforeach()
    set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES} PARENT_SCOPE)
endmacro()

macro(atta_target_common target)
	target_precompile_headers(${target} PRIVATE ${ATTA_PCH})
	target_include_directories(${target} PRIVATE ${ATTA_INCLUDE_DIRS})
	target_compile_options(${target} PRIVATE ${ATTA_OPTIONS})
	target_compile_definitions(${target} PRIVATE ${ATTA_DEFINITIONS})
endmacro()

########## Atta/Extern ##########
list(APPEND ATTA_INCLUDE_DIRS ${ATTA_PATH}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/extern)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/atta)

########## Testing ##########
include(GoogleTest)
enable_testing()

add_executable(atta_test ${ATTA_TEST_SOURCES})
atta_target_common(atta_test)
target_link_libraries(atta_test PRIVATE ${ATTA_LIBS} gtest_main)
gtest_discover_tests(atta_test)

########## Executable ##########
add_executable(atta "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
atta_target_common(atta)
target_link_libraries(atta PRIVATE ${ATTA_LIBS})
