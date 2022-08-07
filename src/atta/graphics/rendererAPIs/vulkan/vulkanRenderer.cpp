//--------------------------------------------------
// Atta Graphics Module
// vulkanRenderer.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/rendererAPIs/vulkan/vulkanRenderer.h>

namespace atta::graphics {
VulkanRenderer::VulkanRenderer() {}

VulkanRenderer::~VulkanRenderer() {}

void VulkanRenderer::beginFrame() {}
void VulkanRenderer::endFrame() {}

void VulkanRenderer::beginRenderPass() {}
void VulkanRenderer::endRenderPass() {}

void VulkanRenderer::renderMesh() {}

void VulkanRenderer::renderQuad() {}

void VulkanRenderer::renderQuad3() {}

void VulkanRenderer::renderCube() {}
} // namespace atta::graphics
