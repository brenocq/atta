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
	list(APPEND ATTA_DEFINITIONS "ATTA_OS_WINDOWS")
	# Force google test to link the runtimes dynamically (to avoid visual studio link error)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	# Fix assert __VA_ARGS__ expansion
	list(APPEND ATTA_OPTIONS /Zc:preprocessor)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
	list(APPEND ATTA_DEFINITIONS "ATTA_OS_MACOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	list(APPEND ATTA_DEFINITIONS "ATTA_OS_LINUX")
else()
	message(SEND_ERROR "Unknown system name: " + CMAKE_SYSTEM_NAME)
endif()

########## Options ##########
if(CMAKE_BUILD_TYPE MATCHES Debug)
	if(MSVC)
		#list(APPEND ATTA_OPTIONS /W4 /WX)
	else()
		list(APPEND ATTA_OPTIONS -Wall -Wextra -Wpedantic -g -pg)#-Werror 
		# Profilling
		list(APPEND ATTA_OPTIONS -fno-omit-frame-pointer)
		# Unused parameter
		list(APPEND ATTA_OPTIONS -Wno-unused-parameter)
		# Math anonymous union 
		list(APPEND ATTA_OPTIONS -Wno-nested-anon-types -Wno-gnu-anonymous-struct)
		# Glad opengl definitions
		list(APPEND ATTA_OPTIONS -Wno-macro-redefined)
		# ImGui warnings
		list(APPEND ATTA_OPTIONS -Wno-invalid-noreturn)
		
	endif()
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
	if(MSVC)
		list(APPEND ATTA_OPTIONS /O2)
	else()
		list(APPEND ATTA_OPTIONS -O3)
	endif()
endif()

########## Output Directories ##########
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)
set(ATTA_TEST_PATH ${CMAKE_BINARY_DIR}/bin/tests CACHE INTERNAL "" FORCE)
set(ATTA_PATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "" FORCE)

########## Precompiled Headers ##########
list(APPEND ATTA_PCH "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/pch.h")

########## Macros ##########
macro(atta_add_libs)
	# Used by systems to register itself to be linked with the atta target
	foreach(_lib ${ARGN})
		list(APPEND ATTA_LIBS ${_lib})
	endforeach()
	set(ATTA_LIBS ${ATTA_LIBS} PARENT_SCOPE)
endmacro()

macro(atta_add_include_dirs)
	# Used by systems to include directories to all targets
	foreach(_include_dir ${ARGN})
		list(APPEND ATTA_INCLUDE_DIRS ${_include_dir})
	endforeach()
	set(ATTA_INCLUDE_DIRS ${ATTA_INCLUDE_DIRS} PARENT_SCOPE)
endmacro()

macro(atta_add_tests)
	# Used by systems to add tests to the atta_test target
	foreach(_test ${ARGN})
		list(APPEND ATTA_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${_test}")
	endforeach()
    set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES} PARENT_SCOPE)
endmacro()

macro(atta_install)
	# Install system headers keeping the directory hierarchy
	set(optionsArgs "")
    set(oneValueArgs "DESTINATION")
    set(multiValueArgs "FILES")
    cmake_parse_arguments(CAS "${optionsArgs}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    foreach(FILE ${CAS_FILES})
        get_filename_component(DIR ${FILE} DIRECTORY)
		install(FILES ${FILE} DESTINATION include/atta/${CAS_DESTINATION}/${DIR})
    endforeach()
endmacro()

macro(atta_target_common target)
	# Common to all targets
	target_precompile_headers(${target} PRIVATE ${ATTA_PCH})
	target_include_directories(${target} PRIVATE ${ATTA_INCLUDE_DIRS})
	target_compile_options(${target} PRIVATE ${ATTA_OPTIONS})
	target_compile_definitions(${target} PRIVATE ${ATTA_DEFINITIONS})
endmacro()

macro(atta_create_local_test target sources libs)
	message(${sources})
	add_executable(${target} ${sources})
	atta_target_common(${target})
	target_link_libraries(${target} PRIVATE ${libs} gtest_main)
	set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ATTA_TEST_PATH})
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

########## Install ##########
install(TARGETS atta atta_test
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
