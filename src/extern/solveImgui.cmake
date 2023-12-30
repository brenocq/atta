set(ATTA_IMGUI_SUPPORT FALSE)
set(ATTA_IMGUI_TARGETS "")

FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG "v1.90-docking"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

atta_log(Info Extern "Fetching ImGui...")
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCE
    ${FETCHCONTENT_BASE_DIR}/imgui-src/imgui.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_demo.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_draw.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_tables.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_widgets.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/backends/imgui_impl_glfw.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/backends/imgui_impl_opengl3.cpp
)
if(ATTA_VULKAN_SUPPORT)
    list(APPEND IMGUI_SOURCE ${FETCHCONTENT_BASE_DIR}/imgui-src/backends/imgui_impl_vulkan.cpp)
endif()

add_library(imgui STATIC
	${IMGUI_SOURCE}
)
target_include_directories(imgui PUBLIC 
    $<BUILD_INTERFACE:${FETCHCONTENT_BASE_DIR}/imgui-src>
    $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/imgui>
)
target_link_libraries(imgui PRIVATE glfw)
if(NOT MSVC)
	target_compile_options(imgui PRIVATE -Wno-invalid-noreturn)
endif()

atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/imgui-src)
atta_add_libs(imgui)

atta_log(Success Extern "ImGui support (source)")
set(ATTA_IMGUI_SUPPORT TRUE)
set(ATTA_IMGUI_TARGETS imgui)
