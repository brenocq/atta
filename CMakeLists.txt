cmake_minimum_required(VERSION 3.12)

project(atta VERSION 0.0.3.0 LANGUAGES CXX C)

OPTION(ATTA_BUILD_TESTS
  "Set to ON to build also the test executables"
  ON)
OPTION(ATTA_WEB_BUILD_MODULE
  "Set to ON to generate only the javascript module"
  OFF)
OPTION(ATTA_BUILD_DOCS
  "Build the documentation"
  OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/atta/cmakeConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/atta/cmakeConfig.h)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # Set flags necessary for the script system to work correctly
    set(CMAKE_CXX_FLAGS "-Wl,--export-dynamic")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--export-dynamic")
endif()

########## Build Types ##########
if(CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND ATTA_DEFINITIONS "ATTA_DEBUG_BUILD")
endif()

########## OS Specific ##########
message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME STREQUAL Windows)#----- Windows build
    set(ATTA_SYSTEM_NAME "Windows")
    list(APPEND ATTA_DEFINITIONS "ATTA_OS_WINDOWS")
    # Force google test to link the runtimes dynamically (to avoid visual studio link error)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # Fix assert __VA_ARGS__ expansion
    list(APPEND ATTA_OPTIONS /Zc:preprocessor)

elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)#----- MacOS build
    set(ATTA_SYSTEM_NAME "MacOS")
    list(APPEND ATTA_DEFINITIONS "ATTA_OS_MACOS")

elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)#----- Linux build
    set(ATTA_SYSTEM_NAME "Linux")
    list(APPEND ATTA_DEFINITIONS "ATTA_OS_LINUX")

elseif(CMAKE_SYSTEM_NAME STREQUAL Emscripten)#----- Web build
    set(ATTA_SYSTEM_NAME "Web")
    list(APPEND ATTA_DEFINITIONS "ATTA_OS_WEB")

    # Configure web build
    set(ATTA_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)

    # Configure emscripten link flags
    set(EMSCRIPTEN_LINK_PROPERTIES "-s USE_WEBGL2=1 -s USE_GLFW=3")
    set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1")
    set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} -s NO_DISABLE_EXCEPTION_CATCHING -s EXIT_RUNTIME=1 -s FORCE_FILESYSTEM=1")
    set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} --preload-file ../resources")
    if(ATTA_WEB_BUILD_MODULE)
        message(STATUS "Building only javascript module")
        set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} -s WASM=1 -s ENVIRONMENT=web -s MODULARIZE=1 -s EXPORT_NAME=AttaModule -s EXPORT_NAME='createModule' -s SINGLE_FILE=1 -s USE_ES6_IMPORT_META=0 --use-preload-plugins")
    endif()
    list(APPEND ATTA_LINK_PROP ${EMSCRIPTEN_LINK_PROPERTIES})
else()#----- Unknown build
    message(SEND_ERROR "Unknown system name: ${CMAKE_SYSTEM_NAME}")
endif()

########## Options ##########
##### Debug #####
if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(MSVC)
        #list(APPEND ATTA_OPTIONS /W4 /WX)
    else()
        list(APPEND ATTA_OPTIONS -g -Wall -Wextra -Wpedantic)
        # Profilling
        list(APPEND ATTA_OPTIONS -fno-omit-frame-pointer)
        # Unused parameter
        list(APPEND ATTA_OPTIONS -Wno-unused-parameter)
        # Math anonymous union 
        #list(APPEND ATTA_OPTIONS -Wno-nested-anon-types -Wno-gnu-anonymous-struct)
        # Glad opengl definitions
        #list(APPEND ATTA_OPTIONS -Wno-macro-redefined)
        # Scripting
        #list(APPEND ATTA_OPTIONS -Wl,--no-undefined -rdynamic)
        #list(APPEND ATTA_OPTIONS -Wl,--export-dynamic)
    endif()
endif()

##### Releaese #####
if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        list(APPEND ATTA_OPTIONS /O2)
    else()
        list(APPEND ATTA_OPTIONS -O3)
    endif()
endif()

##### Default #####
if(MSVC)
else()
    # ImGui warnings
    list(APPEND ATTA_OPTIONS -Wno-invalid-noreturn -Wno-format-security)
    # Useful to ignore
    list(APPEND ATTA_OPTIONS -Wno-missing-field-initializers -Wno-invalid-offsetof -Wno-char-subscripts)
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
    # Used by atta systems to register itself to be linked with the atta target
    foreach(_lib ${ARGN})
        list(APPEND ATTA_LIBS ${_lib})
    endforeach()

    # Update variable
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(ATTA_LIBS ${ATTA_LIBS} PARENT_SCOPE)
    else()
        set(ATTA_LIBS ${ATTA_LIBS})
    endif()
endmacro()

macro(atta_add_include_dirs)
    # Used by atta systems to include directories to all targets
    foreach(_include_dir ${ARGN})
        list(APPEND ATTA_INCLUDE_DIRS ${_include_dir})
    endforeach()

    # Update variable
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(ATTA_INCLUDE_DIRS ${ATTA_INCLUDE_DIRS} PARENT_SCOPE)
    else()
        set(ATTA_INCLUDE_DIRS ${ATTA_INCLUDE_DIRS})
    endif()
endmacro()

macro(atta_add_tests)
    # Used by atta systems to add tests to the atta_test target
    foreach(_test ${ARGN})
        list(APPEND ATTA_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${_test}")
    endforeach()

    # Update variable
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES} PARENT_SCOPE)
    else()
        set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES})
    endif()
endmacro()

macro(atta_target_common target)
    # Common to all targets
    target_precompile_headers(${target} PRIVATE ${ATTA_PCH})
    target_include_directories(${target} PRIVATE ${ATTA_INCLUDE_DIRS})
    target_compile_options(${target} PRIVATE ${ATTA_OPTIONS})
    target_compile_definitions(${target} PRIVATE ${ATTA_DEFINITIONS})
endmacro()

macro(atta_create_local_test target sources libs)
if(ATTA_BUILD_TESTS)
    message(${sources})
    add_executable(${target} ${sources})
    atta_target_common(${target})
    target_link_libraries(${target} PRIVATE ${libs} gtest_main)
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ATTA_TEST_PATH})
endif()
endmacro()

########## Atta/Extern directories ##########
list(APPEND ATTA_INCLUDE_DIRS ${ATTA_PATH}/src)
include(${CMAKE_CURRENT_SOURCE_DIR}/src/extern/extern.cmake)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/atta)


########## Docs ##########
if(ATTA_BUILD_DOCS)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

if(DOXYGEN_FOUND AND ATTA_BUILD_DOCS)
    add_subdirectory(docs)
endif()

########## Testing ##########
if(ATTA_BUILD_TESTS)
    include(GoogleTest)
    enable_testing()

    add_executable(atta_test ${ATTA_TEST_SOURCES})
    atta_target_common(atta_test)

    target_link_libraries(atta_test PRIVATE ${ATTA_LIBS} gtest_main)
    gtest_discover_tests(atta_test)
endif()

########## Install ##########
add_executable(atta "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
atta_target_common(atta)
target_link_libraries(atta PUBLIC ${ATTA_LIBS})
set_target_properties(atta PROPERTIES LINK_FLAGS "${ATTA_LINK_PROP}")
if(ATTA_SYSTEM_NAME MATCHES "Web")
    if(NOT ATTA_WEB_BUILD_MODULE)
        set_target_properties(atta PROPERTIES SUFFIX ".html")
    endif()
endif()

# Install executables
install(TARGETS atta RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
if(ATTA_BUILD_TESTS)
    install(TARGETS atta_test RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# Install includes
# TODO hardcoded for now
install(DIRECTORY src/atta/ DESTINATION include/atta
    FILES_MATCHING REGEX ".*\.(h|inl)$")
install(DIRECTORY src/extern/imgui/ DESTINATION include/atta/extern/imgui
    FILES_MATCHING REGEX ".*\.(h|inl)$")
install(DIRECTORY src/extern/glad/ DESTINATION include/atta/extern/glad
    FILES_MATCHING REGEX ".*\.(h|inl)$")
install(DIRECTORY src/extern/stb_image/ DESTINATION include/atta/extern/stb_image
    FILES_MATCHING REGEX ".*\.(h|inl)$")
install(DIRECTORY src/extern/assimp/ DESTINATION include/atta/extern/assimp
    FILES_MATCHING REGEX ".*\.(h|inl)$")
list(APPEND ATTA_INSTALL_INCLUDE ${CMAKE_INSTALL_PREFIX}/include/atta/extern/imgui)
list(APPEND ATTA_INSTALL_INCLUDE ${CMAKE_INSTALL_PREFIX}/include/atta/extern/glad/include)
list(APPEND ATTA_INSTALL_INCLUDE ${CMAKE_INSTALL_PREFIX}/include/atta/extern/stb_image)
list(APPEND ATTA_INSTALL_INCLUDE ${CMAKE_INSTALL_PREFIX}/include/atta/extern/assimp/include)
set(ATTA_INSTALL_PCH ${CMAKE_INSTALL_PREFIX}/include/atta/pch.h)

# Install resources
install(DIRECTORY resources/ DESTINATION share/atta
    PATTERN "resources/*")

########## Cmake Package ##########
include(CMakePackageConfigHelpers)

# Create version and config files for find_package
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/attaConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)
configure_package_config_file(attaConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/attaConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/atta
)
install(
	FILES 
        "${CMAKE_CURRENT_BINARY_DIR}/attaConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/attaConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/atta
)

# Install cmake targets
#install(TARGETS ${ATTA_LIBS}
#	EXPORT atta_targets
#    INCLUDES DESTINATION src/extern
#)
#install(EXPORT atta_targets
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/atta
#	FILE attaTargets.cmake 
#)

########## CPack (deb package) ##########
#install(TARGETS atta_core
#	COMPONENT atta_component
#)
#
#set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
#set(CPACK_COMPONENTS_ALL atta_component)
#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "BrenoCQ")
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "xorg-dev;libglfw3-dev")
#include(CPack)
