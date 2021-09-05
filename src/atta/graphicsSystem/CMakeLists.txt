cmake_minimum_required(VERSION 3.12)

set(ATTA_GRAPHICS_SYSTEM_SOURCE
	graphicsManager.cpp
	window.cpp
	rendererAPIs/openGL/openGLRenderer.cpp
	layers/layer.cpp
	layers/layerStack.cpp
	layers/internal/uiLayer.cpp
	layers/internal/layer2D.cpp
	layers/internal/editor/editorLayer.cpp
	layers/internal/editor/menuBar.cpp
	layers/internal/editor/dockSpace.cpp
	layers/internal/editor/scenePanel.cpp
)

add_library(atta_graphics_system_lib STATIC
	${ATTA_GRAPHICS_SYSTEM_SOURCE}
)
target_link_libraries(atta_graphics_system_lib PRIVATE glfw glad imgui)
atta_target_common(atta_graphics_system_lib)
atta_add_libs(atta_graphics_system_lib)
