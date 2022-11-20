cmake_minimum_required(VERSION 3.14)
set(FETCHCONTENT_QUIET FALSE)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)# Disable dependency update
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/src/extern")

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

macro(atta_FetchContent_MakeAvailable target)
    FetchContent_GetProperties(${target})
    if(NOT ${target}_POPULATED)
      FetchContent_Populate(${target})
      add_subdirectory(${${target}_SOURCE_DIR} ${${target}_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endmacro()

include(solveResources)
include(solveGoogletest)
include(solveGlfw)
include(solveGlad)
include(solveImgui)
include(solveAssimp)
include(solveStbimage)
include(solveImguizmo)
include(solveImplot)
include(solveBox2d)
include(solveBullet)
include(solveSystemd)
include(solveCurlpp)
