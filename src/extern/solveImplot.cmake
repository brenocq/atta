set(ATTA_IMPLOT_SUPPORT FALSE)
set(ATTA_IMPLOT_TARGETS "")

FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_TAG "3dd7e75c7de3ea6f6030ed7c0b654a31158d6abb"
    GIT_PROGRESS TRUE
)

atta_log(Info Extern "Fetching ImPlot...")
FetchContent_MakeAvailable(implot)

set(IMPLOT_SOURCE
    build/_deps/implot-src/implot.cpp
    build/_deps/implot-src/implot_demo.cpp
    build/_deps/implot-src/implot_items.cpp
)
add_library(implot STATIC
    ${IMPLOT_SOURCE}
)
target_include_directories(implot PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/implot-src>
    $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/implot>
)
target_link_libraries(implot PRIVATE glfw imgui)

atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/implot-src)
atta_add_libs(implot)

atta_log(Success Extern "ImPlot support (source)")
set(ATTA_IMPLOT_SUPPORT TRUE)
set(ATTA_IMPLOT_TARGETS implot)
