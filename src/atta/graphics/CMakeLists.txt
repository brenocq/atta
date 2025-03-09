cmake_minimum_required(VERSION 3.14)

set(ATTA_GRAPHICS_MODULE_SOURCE
    apis/graphicsAPI.cpp
    apis/openGL/openGLAPI.cpp
    apis/openGL/image.cpp
    apis/openGL/framebuffer.cpp
    apis/openGL/renderPass.cpp
    apis/openGL/renderQueue.cpp
    apis/openGL/shader.cpp
    apis/openGL/vertexBuffer.cpp
    apis/openGL/indexBuffer.cpp
    apis/openGL/pipeline.cpp
    apis/openGL/mesh.cpp

    cameras/camera.cpp
    cameras/perspectiveCamera.cpp
    cameras/orthographicCamera.cpp

    compute/entityClick.cpp

    renderers/common/drawerPipeline.cpp
    renderers/common/gridPipeline.cpp
    renderers/common/selectedPipeline.cpp
    renderers/fastRenderer.cpp
    renderers/pbrRenderer.cpp
    renderers/phongRenderer.cpp
    renderers/renderer.cpp

    windows/window.cpp
    windows/nullWindow.cpp
    windows/glfwWindow.cpp

    bufferLayout.cpp
    drawer.cpp
    framebuffer.cpp
    image.cpp
    indexBuffer.cpp
    interface.cpp
    manager.cpp
    mesh.cpp
    pipeline.cpp
    renderPass.cpp
    renderQueue.cpp
    shader.cpp
    vertexBuffer.cpp
)

if(ATTA_VULKAN_SUPPORT)
    list(APPEND ATTA_GRAPHICS_MODULE_SOURCE
        apis/vulkan/buffer.cpp
        apis/vulkan/commandBuffers.cpp
        apis/vulkan/commandPool.cpp
        apis/vulkan/common.cpp
        apis/vulkan/debugMessenger.cpp
        apis/vulkan/descriptorPool.cpp
        apis/vulkan/descriptorSetLayout.cpp
        apis/vulkan/descriptorSets.cpp
        apis/vulkan/device.cpp
        apis/vulkan/fence.cpp
        apis/vulkan/framebuffer.cpp
        apis/vulkan/image.cpp
        apis/vulkan/indexBuffer.cpp
        apis/vulkan/instance.cpp
        apis/vulkan/mesh.cpp
        apis/vulkan/physicalDevice.cpp
        apis/vulkan/pipelineLayout.cpp
        apis/vulkan/pushConstant.cpp
        apis/vulkan/renderPass.cpp
        apis/vulkan/renderQueue.cpp
        apis/vulkan/semaphore.cpp
        apis/vulkan/pipeline.cpp
        apis/vulkan/shader.cpp
        apis/vulkan/stagingBuffer.cpp
        apis/vulkan/surface.cpp
        apis/vulkan/swapChain.cpp
        apis/vulkan/uniformBuffer.cpp
        apis/vulkan/vertexBuffer.cpp
        apis/vulkan/vulkanAPI.cpp
    )
endif()

add_library(atta_graphics_module STATIC
    ${ATTA_GRAPHICS_MODULE_SOURCE}
)
target_link_libraries(atta_graphics_module PUBLIC ${ATTA_GLFW_TARGETS} ${ATTA_GLAD_TARGETS} ${ATTA_VOLK_TARGETS} ${ATTA_IMGUI_TARGETS}
    atta_component_module atta_file_module atta_resource_module atta_ui_module atta_utils)

atta_target_common(atta_graphics_module)
atta_add_libs(atta_graphics_module)
