//--------------------------------------------------
// Atta Graphics Module
// vulkanAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_VULKAN_API_H
#define ATTA_GRAPHICS_APIS_VULKAN_VULKAN_API_H

#include <atta/event/event.h>
#include <atta/graphics/apis/graphicsAPI.h>
#include <atta/graphics/apis/vulkan/commandBuffers.h>
#include <atta/graphics/apis/vulkan/commandPool.h>
#include <atta/graphics/apis/vulkan/debugMessenger.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/fence.h>
#include <atta/graphics/apis/vulkan/framebuffer.h>
#include <atta/graphics/apis/vulkan/indexBuffer.h>
#include <atta/graphics/apis/vulkan/instance.h>
#include <atta/graphics/apis/vulkan/physicalDevice.h>
#include <atta/graphics/apis/vulkan/pipeline.h>
#include <atta/graphics/apis/vulkan/renderPass.h>
#include <atta/graphics/apis/vulkan/semaphore.h>
#include <atta/graphics/apis/vulkan/stagingBuffer.h>
#include <atta/graphics/apis/vulkan/surface.h>
#include <atta/graphics/apis/vulkan/swapChain.h>
#include <atta/graphics/apis/vulkan/vertexBuffer.h>
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

class VulkanAPI final : public GraphicsAPI {
  public:
    VulkanAPI(std::shared_ptr<Window> window);
    ~VulkanAPI();

    void startUp() override;
    void shutDown() override;
    void waitDevice() override;

    void beginFrame() override;
    void endFrame() override;

    void renderMesh(StringId meshSid) override;
    void renderQuad() override;
    void renderQuad3() override;
    void renderCube() override;
    void renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) override;

    void generateCubemap(StringId textureSid, mat4 rotationMatrix = mat4(1.0f)) override;
    void generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) override;
    void generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) override;

    void* getImGuiImage(StringId sid) const override;

    std::shared_ptr<vk::Instance> getInstance() const;
    std::shared_ptr<vk::PhysicalDevice> getPhysicalDevice() const;
    std::shared_ptr<vk::Device> getDevice() const;
    std::shared_ptr<vk::CommandBuffers> getCommandBuffers() const;
    std::shared_ptr<vk::CommandPool> getCommandPool() const;
    std::shared_ptr<vk::RenderPass> getRenderPass() const;
    std::shared_ptr<vk::DescriptorPool> getUiDescriptorPool() const;

  private:
    void recreateSwapChain();
    void onWindowResize(evt::Event e);

    static constexpr int MAX_FRAMES_IN_FLIGHT = 1;
    uint32_t _currFrame;
    uint32_t _imageIndex;
    bool _windowResized;

    std::shared_ptr<vk::Instance> _instance;
    std::shared_ptr<vk::DebugMessenger> _debugMessenger;
    std::shared_ptr<vk::PhysicalDevice> _physicalDevice;
    std::shared_ptr<vk::Device> _device;
    std::shared_ptr<vk::CommandPool> _commandPool;
    std::shared_ptr<vk::CommandBuffers> _commandBuffers;

    // UI
    std::shared_ptr<vk::DescriptorPool> _uiDescriptorPool;

    // Swap chain
    bool _swapChainInitialized;
    std::shared_ptr<vk::Surface> _surface;
    std::shared_ptr<vk::SwapChain> _swapChain;
    std::vector<std::shared_ptr<vk::Framebuffer>> _framebuffers;
    std::shared_ptr<vk::RenderPass> _renderPass;
    std::shared_ptr<vk::Pipeline> _pipeline;
    std::shared_ptr<vk::VertexBuffer> _vertexBuffer;
    std::shared_ptr<vk::IndexBuffer> _indexBuffer;
    std::shared_ptr<vk::StagingBuffer> _stagingBuffer;
    std::vector<std::shared_ptr<vk::Semaphore>> _imageAvailableSemaphores;
    std::vector<std::shared_ptr<vk::Semaphore>> _renderFinishedSemaphores;
    std::vector<std::shared_ptr<vk::Fence>> _inFlightFences;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_APIS_VULKAN_VULKAN_API_H