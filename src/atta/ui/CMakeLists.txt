cmake_minimum_required(VERSION 3.14)

set(ATTA_UI_MODULE_SOURCE
    interface.cpp
    manager.cpp
    editor.cpp

    widgets/align.cpp
    widgets/button.cpp
    widgets/help.cpp
    widgets/image.cpp

    panels/topBar/topBar.cpp
    panels/toolBar/toolBar.cpp
    panels/topBar/localWindows/repoWindow.cpp
    panels/topBar/localWindows/versionWindow.cpp

    windows/timeProfiler/timeProfilerWindow.cpp
    windows/timeProfiler/components/tearDown.cpp
    windows/timeProfiler/components/flameGraph.cpp
    windows/timeProfiler/components/timeline.cpp
    windows/utils/fileSelectionWindow.cpp
    windows/entityWindow.cpp
    windows/graphicsModuleWindow.cpp
    windows/ioModuleWindow.cpp
    windows/logWindow.cpp
    windows/physicsModuleWindow.cpp
    windows/sensorModuleWindow.cpp
    windows/viewportWindows.cpp
    windows/window.cpp

	editor/drawers/physicsDrawer.cpp
	editor/drawers/sensorDrawer.cpp
)

add_library(atta_ui_module STATIC
    ${ATTA_UI_MODULE_SOURCE}
)

target_link_libraries(atta_ui_module PRIVATE atta_graphics_module atta_io_module atta_component_module ${ATTA_IMGUIZMO_TARGETS} ${ATTA_IMPLOT_TARGETS} ${ATTA_IMPLOT3D_TARGETS})
atta_target_common(atta_ui_module)
atta_add_libs(atta_ui_module)
