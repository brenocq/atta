//--------------------------------------------------
// Atta Graphics Module
// renderPass.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/renderPass.h>

#include <atta/graphics/apis/vulkan/framebuffer.h>
#include <atta/graphics/apis/vulkan/image.h>
#include <atta/graphics/apis/vulkan/vulkanAPI.h>
#include <atta/graphics/interface.h>

namespace atta::graphics::vk {

RenderPass::RenderPass(const graphics::RenderPass::CreateInfo& info) : graphics::RenderPass(info), _device(common::getDevice()) {
    // Color attachment
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = vk::Image::convertFormat(_framebuffer->getImage(0)->getFormat());
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // TODO _device->getMsaaSamples();
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

    // Subpass dependency
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Create render pass
    std::vector<VkAttachmentDescription> attachments = {colorAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(_device->getHandle(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::RenderPass", "Failed to create render pass!");
}

RenderPass::~RenderPass() {
    if (_renderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(_device->getHandle(), _renderPass, nullptr);
}

void RenderPass::begin(bool clear) {
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _renderPass;
    renderPassInfo.framebuffer = std::dynamic_pointer_cast<Framebuffer>(_framebuffer)->getHandle();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {_framebuffer->getWidth(), _framebuffer->getHeight()};

    vec4 color = _framebuffer->getClearColor();
    VkClearValue clearColor = {{color.x, color.y, color.z, color.w}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    VkCommandBuffer commandBuffer = common::getCommandBuffers()->getCurrent();
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::end() {
    VkCommandBuffer commandBuffer = common::getCommandBuffers()->getCurrent();
    vkCmdEndRenderPass(commandBuffer);
}

VkRenderPass RenderPass::getHandle() const { return _renderPass; }

void RenderPass::setFramebuffer(std::shared_ptr<Framebuffer> framebuffer) { _framebuffer = framebuffer; }

} // namespace atta::graphics::vk
