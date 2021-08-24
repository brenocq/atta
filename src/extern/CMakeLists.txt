cmake_minimum_required(VERSION 3.12)

########## googletest ##########
option(BUILD_GMOCK OFF)
option(INSTALL_GTEST OFF)
add_subdirectory(googletest)

########## glfw ##########
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
atta_add_libs(glfw)
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)

########## glad ##########
add_subdirectory(glad)
atta_add_libs(glad)
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

########## imgui ##########
set(IMGUI_SOURCE
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
)
add_library(imgui STATIC
	${IMGUI_SOURCE}
)
atta_add_libs(imgui)
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/imgui)
