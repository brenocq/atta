//--------------------------------------------------
// Atta Graphics Module
// vulkanAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/vulkanAPI.h>

#include "vulkan/vulkan.h"

namespace atta::graphics {

VulkanAPI::VulkanAPI() : GraphicsAPI(GraphicsAPI::VULKAN) {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::cout << extensionCount << " extensions supported\n";
}

VulkanAPI::~VulkanAPI() {}

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

} // namespace atta::graphics
