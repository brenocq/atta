cmake_minimum_required(VERSION 3.14)
set(FETCHCONTENT_QUIET FALSE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/src/extern")

include(solveGoogletest)
include(solveGlfw)
include(solveGlad)
include(solveImgui)
include(solveAssimp)
include(solveStbimage)
include(solveImguizmo)
include(solveBox2d)
include(solveBullet)
include(solveCppnetlib)
include(solveSystemd)
