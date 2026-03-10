cmake_minimum_required(VERSION 3.14)
project(cube-factory)

find_package(atta 0.3.6 REQUIRED)

atta_add_target(cubeScript "src/cubeScript.cpp")
