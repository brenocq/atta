cmake_minimum_required(VERSION 3.14)
project(simple-script)

find_package(atta 0.3.6 REQUIRED)

# The CMakeLists.txt lists all scripts that will be exposed to the atta.
# This example creates one project script: projectScript
# and two entity scripts: cubeScript and lightScript

# Create project script target
atta_add_target(projectScript "src/projectScript.cpp")

# Create cube script target
atta_add_target(cubeScript "src/cubeScript.cpp")

# Create light script target
atta_add_target(lightScript "src/lightScript.cpp")
