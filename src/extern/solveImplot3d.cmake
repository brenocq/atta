set(ATTA_IMPLOT3D_SUPPORT FALSE)
set(ATTA_IMPLOT3D_TARGETS "")

FetchContent_Declare(
    implot3d
    GIT_REPOSITORY "https://github.com/brenocq/implot3d"
    GIT_TAG "main"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

atta_log(Info Extern "Fetching ImPlot3D...")
FetchContent_MakeAvailable(implot3d)

set(IMPLOT3D_SOURCE
    ${FETCHCONTENT_BASE_DIR}/implot3d-src/implot3d.cpp
    ${FETCHCONTENT_BASE_DIR}/implot3d-src/implot3d_demo.cpp
    ${FETCHCONTENT_BASE_DIR}/implot3d-src/implot3d_items.cpp
    ${FETCHCONTENT_BASE_DIR}/implot3d-src/implot3d_meshes.cpp
)
add_library(implot3d STATIC
    ${IMPLOT3D_SOURCE}
)
target_include_directories(implot3d PUBLIC
    $<BUILD_INTERFACE:${FETCHCONTENT_BASE_DIR}/implot3d-src>
    $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/implot3d>
)
target_link_libraries(implot3d PRIVATE glfw imgui)

atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/implot3d-src)
atta_add_libs(implot3d)

atta_log(Success Extern "ImPlot support (source)")
set(ATTA_IMPLOT3D_SUPPORT TRUE)
set(ATTA_IMPLOT3D_TARGETS implot3d)
