set(ATTA_IMGUI_SUPPORT FALSE)
set(ATTA_IMGUI_TARGETS "")

FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG "e900ca355e3c7be7672e25800af346f1179e91d8"
    GIT_PROGRESS TRUE
)

atta_log(Info Extern "Fetching ImGui...")
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCE
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui.cpp
	${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_demo.cpp
	${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_draw.cpp
	${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_tables.cpp
	${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_widgets.cpp
	${CMAKE_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_glfw.cpp
	${CMAKE_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC
	${IMGUI_SOURCE}
)
target_include_directories(imgui PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/imgui-src>
    $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/imgui>
)
target_link_libraries(imgui PRIVATE glfw)
if(NOT MSVC)
	target_compile_options(imgui PRIVATE -Wno-invalid-noreturn)
endif()

atta_add_include_dirs(${CMAKE_BINARY_DIR}/_deps/imgui-src)
atta_add_libs(imgui)

atta_log(Success Extern "ImGui support (source)")
set(ATTA_IMGUI_SUPPORT TRUE)
set(ATTA_IMGUI_TARGETS imgui)
