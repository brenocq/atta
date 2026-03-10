cmake_minimum_required(VERSION 3.14)
project(material-resource)

find_package(atta 0.3.6 REQUIRED)

# Download resources
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(resources
    URL "https://storage.googleapis.com/atta-resources/tutorial/materials.zip"
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources
)
FetchContent_Populate(resources)
