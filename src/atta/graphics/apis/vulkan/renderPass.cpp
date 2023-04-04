//--------------------------------------------------
// Atta Graphics Module
// renderPass.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/renderPass.h>

namespace atta::graphics::vk {

RenderPass::RenderPass(const graphics::RenderPass::CreateInfo& info, std::shared_ptr<Device> device) : graphics::RenderPass(info), _device(device) {
    // Color attachment
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM; // TODO _renderPass->getFramebuffer()->getFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;   // TODO _device->getMsaaSamples();
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Subpass
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // Create render pass
    std::array<VkAttachmentDescription, 3> attachments = {colorAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(_device->getHandle(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::RenderPass", "Failed to create render pass!");
}

RenderPass::~RenderPass() {
    if (_renderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(_device->getHandle(), _renderPass, nullptr);
}

VkRenderPass RenderPass::getHandle() const { return _renderPass; }

} // namespace atta::graphics::vk
