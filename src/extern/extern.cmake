cmake_minimum_required(VERSION 3.14)
set(FETCHCONTENT_QUIET FALSE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/src/extern")

include(solveGoogletest)# google_test
include(solveGlfw)# glad
include(solveGlad)# glad
include(solveImgui)# imgui
include(solveAssimp)# assimp
include(solveStbimage)# stb_image
include(solveImguizmo)# imguizmo
include(solveBox2d)# box2d
include(solveBullet)# bullet
include(solveCpprestsdk)# cpprestsdk
