set(ATTA_IMPLOT_SUPPORT FALSE)
set(ATTA_IMPLOT_TARGETS "")

FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_TAG "v0.16"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

atta_log(Info Extern "Fetching ImPlot...")
FetchContent_MakeAvailable(implot)

set(IMPLOT_SOURCE
    ${FETCHCONTENT_BASE_DIR}/implot-src/implot.cpp
    ${FETCHCONTENT_BASE_DIR}/implot-src/implot_demo.cpp
    ${FETCHCONTENT_BASE_DIR}/implot-src/implot_items.cpp
)
add_library(implot STATIC
    ${IMPLOT_SOURCE}
)
target_include_directories(implot PUBLIC 
    $<BUILD_INTERFACE:${FETCHCONTENT_BASE_DIR}/implot-src>
    $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/implot>
)
target_link_libraries(implot PRIVATE glfw imgui)

atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/implot-src)
atta_add_libs(implot)

atta_log(Success Extern "ImPlot support (source)")
set(ATTA_IMPLOT_SUPPORT TRUE)
set(ATTA_IMPLOT_TARGETS implot)
