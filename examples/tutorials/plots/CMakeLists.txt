cmake_minimum_required(VERSION 3.14)
project(plots)

find_package(atta 0.3.6 REQUIRED)

atta_add_target(project_script "src/projectScript.cpp")
target_link_libraries(project_script PRIVATE imgui implot implot3d)
