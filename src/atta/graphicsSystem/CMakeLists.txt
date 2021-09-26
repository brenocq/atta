cmake_minimum_required(VERSION 3.12)

set(ATTA_GRAPHICS_SYSTEM_SOURCE
	graphicsManager.cpp
	drawer.cpp
	window.cpp
	viewport.cpp

	image.cpp
	framebuffer.cpp
	renderPass.cpp
	shaderUniform.cpp
	shader.cpp
	shaderGroup.cpp
	vertexBuffer.cpp
	indexBuffer.cpp
	pipeline.cpp

	rendererAPI.cpp
	rendererAPIs/openGL/openGLRenderer.cpp
	rendererAPIs/openGL/openGLImage.cpp
	rendererAPIs/openGL/openGLFramebuffer.cpp
	rendererAPIs/openGL/openGLRenderPass.cpp
	rendererAPIs/openGL/openGLShader.cpp
	rendererAPIs/openGL/openGLShaderGroup.cpp
	rendererAPIs/openGL/openGLVertexBuffer.cpp
	rendererAPIs/openGL/openGLIndexBuffer.cpp
	rendererAPIs/openGL/openGLPipeline.cpp
	rendererAPIs/openGL/openGLMesh.cpp

	renderers/renderer.cpp
	renderers/fastRenderer.cpp
	renderers/pbrRenderer.cpp
	renderers/phongRenderer.cpp

	cameras/camera.cpp
	cameras/perspectiveCamera.cpp
	cameras/orthographicCamera.cpp

	layers/layer.cpp
	layers/layerStack.cpp
	layers/internal/uiLayer.cpp
	layers/internal/layer2D.cpp
	layers/internal/simulationLayer.cpp
	layers/internal/editor/editorLayer.cpp
	layers/internal/editor/menuBar.cpp
	layers/internal/editor/dockSpace.cpp
	layers/internal/editor/scenePanel.cpp
)

add_library(atta_graphics_system STATIC
	${ATTA_GRAPHICS_SYSTEM_SOURCE}
)
target_link_libraries(atta_graphics_system PRIVATE glfw glad imgui)
atta_target_common(atta_graphics_system)
atta_add_libs(atta_graphics_system)
