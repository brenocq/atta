//--------------------------------------------------
// Atta Graphics Module
// vulkanAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_VULKAN_API_H
#define ATTA_GRAPHICS_APIS_VULKAN_VULKAN_API_H
#include <atta/graphics/apis/graphicsAPI.h>
#include <atta/graphics/apis/vulkan/instance.h>
#include <atta/graphics/apis/vulkan/physicalDevice.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/debugMessenger.h>
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

class VulkanAPI final : public GraphicsAPI {
  public:
    VulkanAPI();
    ~VulkanAPI();

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

  private:
    std::shared_ptr<vk::Instance> _instance;
    std::shared_ptr<vk::DebugMessenger> _debugMessenger;
    std::shared_ptr<vk::PhysicalDevice> _physicalDevice;
    std::shared_ptr<vk::Device> _device;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_APIS_VULKAN_VULKAN_API_H
