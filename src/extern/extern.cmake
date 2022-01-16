cmake_minimum_required(VERSION 3.14)
set(FETCHCONTENT_QUIET FALSE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/src/extern")

include(findGoogletest)# google_test
include(findGlfw)# glad
include(findGlad)# glad
include(findImgui)# imgui
include(findAssimp)# assimp
include(findStbimage)# stb_image
include(findImguizmo)# imguizmo
include(findBox2d)# box2d
include(findBullet)# bullet
include(findCpprestsdk)# cpprestsdk
