cmake_minimum_required(VERSION 3.14)

set(ATTA_UI_MODULE_SOURCE
    interface.cpp
    manager.cpp
    editor.cpp

    widgets/align.cpp
    widgets/button.cpp
    widgets/help.cpp
    widgets/image.cpp

    editor/dockSpace.cpp
    editor/window.cpp

    editor/topBar/topBar.cpp
    editor/toolBar/toolBar.cpp
    editor/topBar/localWindows/repoWindow.cpp
    editor/topBar/localWindows/versionWindow.cpp

    editor/windows/entityWindow.cpp
    editor/windows/logWindow.cpp
    editor/windows/viewportWindows.cpp
    editor/windows/utils/fileSelectionWindow.cpp

    editor/moduleWindows/graphicsModuleWindow.cpp
    editor/moduleWindows/ioModuleWindow.cpp
    editor/moduleWindows/physicsModuleWindow.cpp
    editor/moduleWindows/sensorModuleWindow.cpp

    editor/tools/timeProfiler/timeProfilerWindow.cpp
    editor/tools/timeProfiler/components/tearDown.cpp
    editor/tools/timeProfiler/components/flameGraph.cpp
    editor/tools/timeProfiler/components/timeline.cpp

	editor/drawers/physicsDrawer.cpp
	editor/drawers/sensorDrawer.cpp
)

add_library(atta_ui_module STATIC
    ${ATTA_UI_MODULE_SOURCE}
)

target_link_libraries(atta_ui_module PRIVATE atta_graphics_module atta_io_module atta_component_module ${ATTA_IMGUIZMO_TARGETS} ${ATTA_IMPLOT_TARGETS} ${ATTA_IMPLOT3D_TARGETS})
atta_target_common(atta_ui_module)
atta_add_libs(atta_ui_module)
