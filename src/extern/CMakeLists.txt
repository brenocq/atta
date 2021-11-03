cmake_minimum_required(VERSION 3.12)

########## googletest ##########
if(ATTA_BUILD_TESTS)
    option(BUILD_GMOCK OFF)
    option(INSTALL_GTEST OFF)
    add_subdirectory(googletest)
endif()

########## glfw ##########
if(NOT ATTA_SYSTEM_NAME MATCHES "Web")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw)
    atta_add_libs(glfw)
    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
endif()

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
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC
	${IMGUI_SOURCE}
)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(imgui PRIVATE glfw)
if(NOT MSVC)
	target_compile_options(imgui PRIVATE -Wno-invalid-noreturn)
endif()
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/imgui)
atta_add_libs(imgui)

########## assimp ##########
include(FetchContent)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY  https://github.com/assimp/assimp.git
  GIT_TAG         6841397798051919a23a3fbe1405fd934fb5f39c
)

FetchContent_GetProperties(assimp)
string(TOLOWER "assimp" lcName)
if(NOT ${lcName}_POPULATED)
  FetchContent_Populate(assimp)

  set(BUILD_SHARED_LIBS FALSE CACHE INTERNAL "" FORCE)

  # Turn off assimp options to improve build time and reduce size
  set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
  set(ASSIMP_IGNORE_GIT_HASH ON CACHE INTERNAL "" FORCE)
  set(ASSIMP_INSTALL_PDB FALSE CACHE INTERNAL "" FORCE )
  set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL "" FORCE)

  add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR})
endif()

########## stb_image ##########
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
