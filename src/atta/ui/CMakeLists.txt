cmake_minimum_required(VERSION 3.14)

set(ATTA_UI_MODULE_SOURCE
    interface.cpp
    manager.cpp

    layers/uiLayer.cpp
    layers/editor/editorLayer.cpp
    layers/editor/dockSpace.cpp
    layers/editor/window.cpp

    layers/editor/topBar/topBar.cpp
    layers/editor/toolBar/toolBar.cpp
    layers/editor/topBar/localWindows/repoWindow.cpp
    layers/editor/topBar/localWindows/versionWindow.cpp

    layers/editor/windows/entityWindow.cpp
    layers/editor/windows/logWindow.cpp
    layers/editor/windows/viewportWindows.cpp
    layers/editor/windows/utils/fileSelectionWindow.cpp

    layers/editor/moduleWindows/graphicsModuleWindow.cpp
    layers/editor/moduleWindows/ioModuleWindow.cpp
    layers/editor/moduleWindows/physicsModuleWindow.cpp

	layers/editor/drawers/physicsDrawer.cpp
	layers/editor/drawers/sensorDrawer.cpp
)

add_library(atta_ui_module STATIC
    ${ATTA_UI_MODULE_SOURCE}
)

target_link_libraries(atta_ui_module PRIVATE atta_graphics_module atta_io_module atta_component_module ${ATTA_IMGUIZMO_TARGETS} ${ATTA_IMPLOT_TARGETS})
atta_target_common(atta_ui_module)
atta_add_libs(atta_ui_module)
