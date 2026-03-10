cmake_minimum_required(VERSION 3.14)
project(ray-cast)

find_package(atta 0.3.6 REQUIRED)

atta_add_target(projectScript "src/projectScript.cpp")
