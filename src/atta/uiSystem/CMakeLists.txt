cmake_minimum_required(VERSION 3.12)

set(ATTA_UI_SYSTEM_SOURCE
	uiManager.cpp

	layers/uiLayer.cpp
	layers/editor/editorLayer.cpp
	layers/editor/menuBar/menuBar.cpp
	layers/editor/dockSpace.cpp
	layers/editor/scenePanel.cpp
)

add_library(atta_ui_system STATIC
	${ATTA_UI_SYSTEM_SOURCE}
)
target_link_libraries(atta_ui_system PRIVATE atta_graphics_system)
atta_target_common(atta_ui_system)
atta_add_libs(atta_ui_system)
