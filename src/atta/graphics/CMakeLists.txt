cmake_minimum_required(VERSION 3.14)

set(ATTA_GRAPHICS_MODULE_SOURCE
    manager.cpp
    drawer.cpp
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

    windows/window.cpp
    windows/nullWindow.cpp
    windows/glfwWindow.cpp

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
    renderers/common/selectedPipeline.cpp
    renderers/common/drawerPipeline.cpp

    compute/entityClick.cpp

    cameras/camera.cpp
    cameras/perspectiveCamera.cpp
    cameras/orthographicCamera.cpp

    layers/layer.cpp
    layers/layerStack.cpp
)

add_library(atta_graphics_module STATIC
    ${ATTA_GRAPHICS_MODULE_SOURCE}
)
target_link_libraries(atta_graphics_module PUBLIC ${ATTA_GLFW_TARGETS} ${ATTA_GLAD_TARGETS} ${ATTA_IMGUI_TARGETS} 
    atta_component_module atta_file_module atta_resource_module atta_ui_module atta_utils)

atta_target_common(atta_graphics_module)
atta_add_libs(atta_graphics_module)
