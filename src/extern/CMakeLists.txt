cmake_minimum_required(VERSION 3.12)

########## googletest ##########
option(BUILD_GMOCK OFF)
option(INSTALL_GTEST OFF)
add_subdirectory(googletest)
#set(GTEST_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include PARENT_SCOPE)
