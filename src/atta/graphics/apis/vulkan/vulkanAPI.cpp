//--------------------------------------------------
// Atta Graphics Module
// vulkanAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/vulkanAPI.h>

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/shaderGroup.h>

namespace atta::graphics {

VulkanAPI::VulkanAPI(std::shared_ptr<Window> window) : GraphicsAPI(GraphicsAPI::VULKAN, window) {
    _instance = std::make_shared<vk::Instance>();
#ifdef ATTA_DEBUG_BUILD
    _debugMessenger = std::make_shared<vk::DebugMessenger>(_instance);
#endif
    _physicalDevice = std::make_shared<vk::PhysicalDevice>(_instance);
    _device = std::make_shared<vk::Device>(_physicalDevice);
    _surface = std::make_shared<vk::Surface>(_instance, _window);
    _swapChain = std::make_shared<vk::SwapChain>(_device, _surface);
    _commandPool = std::make_shared<vk::CommandPool>(_device);
    _commandBuffers = std::make_shared<vk::CommandBuffers>(_device, _commandPool, 1);

    graphics::ShaderGroup::CreateInfo info;
    info.shaderPaths = {"shaders/triangle/shader-spv.vert", "shaders/triangle/shader-spv.frag"};
    std::shared_ptr<vk::ShaderGroup> shaderGroup = std::make_shared<vk::ShaderGroup>(info, _device);

    LOG_SUCCESS("gfx::VulkanAPI", "Initialized");
}

VulkanAPI::~VulkanAPI() {
    _commandBuffers.reset();
    _commandPool.reset();
    _swapChain.reset();
    _surface.reset();
    _device.reset();
    _physicalDevice.reset();
    _debugMessenger.reset();
    _instance.reset();
}

void VulkanAPI::beginFrame() {}

void VulkanAPI::endFrame() {}

void VulkanAPI::renderMesh(StringId meshSid) {}

void VulkanAPI::renderQuad() {}

void VulkanAPI::renderQuad3() {}

void VulkanAPI::renderCube() {}

void VulkanAPI::renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) {}

void VulkanAPI::generateCubemap(StringId textureSid, mat4 rotationMatrix) {}

void VulkanAPI::generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) {}

void VulkanAPI::generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) {}

void* VulkanAPI::getImGuiImage(StringId sid) const { return nullptr; }

std::shared_ptr<vk::Device> VulkanAPI::getDevice() const { return _device; }
std::shared_ptr<vk::CommandPool> VulkanAPI::getCommandPool() const { return _commandPool; }

} // namespace atta::graphics
