cmake_minimum_required(VERSION 3.12)

set(ATTA_UI_SYSTEM_SOURCE
    uiManager.cpp

    layers/uiLayer.cpp
    layers/editor/editorLayer.cpp
    layers/editor/dockSpace.cpp
    layers/editor/window.cpp

    layers/editor/topBar/topBar.cpp
    layers/editor/topBar/localWindows/repoWindow.cpp

    layers/editor/windows/entityWindow.cpp

    layers/editor/systemWindows/ioSystemWindow.cpp
    layers/editor/systemWindows/physicsSystemWindow.cpp

	layers/editor/drawers/physicsDrawer.cpp
)

add_library(atta_ui_system STATIC
    ${ATTA_UI_SYSTEM_SOURCE}
)

target_link_libraries(atta_ui_system PRIVATE atta_graphics_system atta_io_system atta_cv imguizmo atta_component_system ${ATTA_CPPRESTSDK_TARGETS})
atta_target_common(atta_ui_system)
atta_add_libs(atta_ui_system)
