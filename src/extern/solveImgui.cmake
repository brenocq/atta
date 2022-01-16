set(ATTA_IMGUI_SUPPORT FALSE)
set(ATTA_IMGUI_TARGETS "")

FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG "ec1e57ed4a616ee42f4711b4a955bc68b9c1f463"
    GIT_PROGRESS TRUE
)

atta_log(Info Extern "Fetching ImGui...")
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCE
	build/_deps/imgui-src/imgui.cpp
	build/_deps/imgui-src/imgui_demo.cpp
	build/_deps/imgui-src/imgui_draw.cpp
	build/_deps/imgui-src/imgui_tables.cpp
	build/_deps/imgui-src/imgui_widgets.cpp
	build/_deps/imgui-src/backends/imgui_impl_glfw.cpp
	build/_deps/imgui-src/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC
	${IMGUI_SOURCE}
)
target_include_directories(imgui PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src>
    $<INSTALL_INTERFACE:include/atta/extern/imgui>
)
target_link_libraries(imgui PRIVATE glfw)
if(NOT MSVC)
	target_compile_options(imgui PRIVATE -Wno-invalid-noreturn)
endif()

atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src)
atta_add_libs(imgui)

atta_log(Success Extern "ImGui support (source)")
set(ATTA_IMGUI_SUPPORT TRUE)
set(ATTA_IMGUI_TARGETS imgui)
