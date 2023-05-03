cmake_minimum_required(VERSION 3.14)

project(atta VERSION 0.4.0.0 LANGUAGES CXX C CUDA)

OPTION(ATTA_BUILD_TESTS
    "Set to ON to build also the test executables"
    ON)
OPTION(ATTA_WEB_BUILD_MODULE
    "Set to ON to generate only the javascript module"
    OFF)
OPTION(ATTA_BUILD_DOCS
    "Build the documentation"
    OFF)
option(ATTA_STATIC_PROJECT_FILE 
    "Project to be linked statically to atta" 
    "")
option(ATTA_PROFILE 
    "Set to ON to enable code profiling" 
    ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(ATTA_VERSION_SAFE atta-${CMAKE_PROJECT_VERSION})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/atta/cmakeConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/atta/cmakeConfig.h)

# Set flags necessary for the script system to work correctly
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-Wl,--export-dynamic")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--export-dynamic")
endif()

########## Macros ##########
include(cmake/macros.cmake)

########## Log ##########
include(cmake/log.cmake)

########## Build Types ##########
atta_log(Info "Main" "cmake build type: ${CMAKE_BUILD_TYPE}")
atta_log(Info "Main" "cmake build dir: ${CMAKE_BINARY_DIR}")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND ATTA_DEFINITIONS "ATTA_DEBUG_BUILD")
endif()

if(ATTA_STATIC_PROJECT_FILE)
    set(ATTA_STATIC_PROJECT TRUE)
    list(APPEND ATTA_DEFINITIONS "ATTA_STATIC_PROJECT")
    atta_log(Info "Main" "Building atta statically linked to ${ATTA_STATIC_PROJECT_FILE}")
else()
    set(ATTA_STATIC_PROJECT FALSE)
endif()

if(ATTA_PROFILE)
    list(APPEND ATTA_DEFINITIONS "ATTA_PROFILE")
endif()

########## OS Specific ##########
atta_log(Info "Main" "cmake system name: ${CMAKE_SYSTEM_NAME}")
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
    set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} -s NO_DISABLE_EXCEPTION_CATCHING -s EXIT_RUNTIME=1")
    set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@/resources/")


    if(ATTA_STATIC_PROJECT_FILE)
        # Copy project filee
        configure_file(${ATTA_STATIC_PROJECT_FILE} project/project.atta COPYONLY)

        get_filename_component(projectDir ${ATTA_STATIC_PROJECT_FILE} DIRECTORY)
        if(EXISTS "${projectDir}/resources")
            file(COPY "${projectDir}/resources" DESTINATION "project")
            set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} --preload-file project/resources")
        endif()

        set(EMSCRIPTEN_LINK_PROPERTIES "${EMSCRIPTEN_LINK_PROPERTIES} --preload-file project/project.atta")
    endif()

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
        # Script C linkage
        list(APPEND ATTA_OPTIONS -Wno-return-type-c-linkage)
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

########## Atta/Extern directories ##########
include(FetchContent)
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

########## Executable ##########
add_executable(atta "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
atta_target_common(atta)
target_link_libraries(atta PUBLIC ${ATTA_LIBS})

########## Static project ##########
if(ATTA_STATIC_PROJECT)
    # Instead of looking for atta installed in the machine, use the one being built (must for web build)
    set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_MODULE_PATH}")
    file(WRITE ${CMAKE_BINARY_DIR}/Findatta.cmake "include(attaConfig)")
    set(ATTA_INSTALL_LIBRARIES ${ATTA_LIBS})
    set(ATTA_INSTALL_INCLUDE_DIRS ${ATTA_INCLUDE_DIRS})
    set(ATTA_INSTALL_PCH ${ATTA_PCH})
    configure_file(cmake/attaConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/attaConfig.cmake" @ONLY)

    get_filename_component(projectDirectory ${ATTA_STATIC_PROJECT_FILE} DIRECTORY)
    add_subdirectory(${projectDirectory} ${projectDirectory}/build)
    # The project targets will are saved to ATTA_PROJECT_TARGETS (atta_add_target macro)
    target_link_libraries(atta PUBLIC ${ATTA_PROJECT_TARGETS})

    # Populate ATTA_STATIC_PROJECT_SCRIPTS to write includes to scripts.h file
    set(ATTA_STATIC_PROJECT_SCRIPTS "")
    foreach(projHeader IN LISTS ATTA_PROJECT_HEADERS)
        if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/${possible}")
            set(ATTA_STATIC_PROJECT_SCRIPTS "${ATTA_STATIC_PROJECT_SCRIPTS}#include \"${projHeader}\"\n")
        endif()
    endforeach()
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/script/scripts.h.in" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/atta/script/scripts.h"
)

# Web build
if(ATTA_SYSTEM_NAME MATCHES "Web")
    set_target_properties(atta PROPERTIES LINK_FLAGS "${ATTA_LINK_PROP}")
    if(NOT ATTA_WEB_BUILD_MODULE)
        set_target_properties(atta PROPERTIES SUFFIX ".html")
    endif()
endif()

########## Install/Uninstall ##########
# Do not allow to install web build and static build
if(NOT (ATTA_SYSTEM_NAME MATCHES "Web") AND NOT ATTA_STATIC_PROJECT_FILE)
    ########## Install files ##########

    # Install atta executable
    install(TARGETS atta 
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ATTA_VERSION_SAFE}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ATTA_VERSION_SAFE}
    )
    # Install atta-X.X.X.X executable
    install(FILES $<TARGET_FILE:atta> TYPE BIN RENAME ${ATTA_VERSION_SAFE} PERMISSIONS WORLD_EXECUTE)

    # Install atta_test-X.X.X.X executable
    if(ATTA_BUILD_TESTS)
        install(TARGETS atta_test RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
        install(FILES $<TARGET_FILE:atta_test> TYPE BIN RENAME atta_test-${CMAKE_PROJECT_VERSION} PERMISSIONS WORLD_EXECUTE)
    endif()

    # Install atta includes
    install(DIRECTORY src/atta/ DESTINATION include/${ATTA_VERSION_SAFE}/atta
        FILES_MATCHING REGEX ".*\.(h|inl)$")

    # Install extern includes
    install(DIRECTORY ${CMAKE_BINARY_DIR}/_deps/imgui-src/ DESTINATION include/${ATTA_VERSION_SAFE}/extern/imgui
        FILES_MATCHING REGEX ".*\.(h|inl)$")
    install(DIRECTORY ${CMAKE_BINARY_DIR}/_deps/implot-src/ DESTINATION include/${ATTA_VERSION_SAFE}/extern/implot
        FILES_MATCHING REGEX ".*\.(h|cpp)$")
    install(DIRECTORY ${CMAKE_BINARY_DIR}/_deps/bullet3-src/ DESTINATION include/${ATTA_VERSION_SAFE}/extern/bullet3
        FILES_MATCHING REGEX ".*\.(h|cpp)$")
    install(DIRECTORY src/extern/glad/ DESTINATION include/${ATTA_VERSION_SAFE}/extern/glad
        FILES_MATCHING REGEX ".*\.(h|inl)$")
    install(DIRECTORY src/extern/stb_image/ DESTINATION include/${ATTA_VERSION_SAFE}/extern/stb_image
        FILES_MATCHING REGEX ".*\.(h|inl)$")

    # Set install variables
    list(APPEND ATTA_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/${ATTA_VERSION_SAFE})
    list(APPEND ATTA_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/${ATTA_VERSION_SAFE}/extern/imgui)
    list(APPEND ATTA_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/${ATTA_VERSION_SAFE}/extern/implot)
    list(APPEND ATTA_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/${ATTA_VERSION_SAFE}/extern/bullet3/src)
    list(APPEND ATTA_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/${ATTA_VERSION_SAFE}/extern/glad/include)
    list(APPEND ATTA_INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/${ATTA_VERSION_SAFE}/extern/stb_image)
    set(ATTA_INSTALL_PCH ${CMAKE_INSTALL_PREFIX}/include/${ATTA_VERSION_SAFE}/atta/pch.h)
    set(ATTA_INSTALL_LIBRARIES ${ATTA_LIBS})

    # Install resources
    install(DIRECTORY resources/ DESTINATION share/${ATTA_VERSION_SAFE} PATTERN "resources/*")

    # Uninstall target
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/attaUninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/attaUninstall.cmake"
        @ONLY)
    add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/attaUninstall.cmake")

    ########## Cmake Package ##########
    include(CMakePackageConfigHelpers)

    # Create version and config files for find_package
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/attaConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    configure_package_config_file(cmake/attaConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/attaConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ATTA_VERSION_SAFE}
    )
    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/attaConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/attaConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ATTA_VERSION_SAFE}
    )

    # Install atta libraries
    install(TARGETS ${ATTA_LIBS}
        EXPORT atta_targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${ATTA_VERSION_SAFE}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${ATTA_VERSION_SAFE}
    )
    # Write cmake targets
    install(EXPORT atta_targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${ATTA_VERSION_SAFE}
        FILE attaTargets.cmake
    )
endif()
