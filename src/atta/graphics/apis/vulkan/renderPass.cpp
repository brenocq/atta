//--------------------------------------------------
// Atta Graphics Module
// renderPass.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/renderPass.h>

#include <atta/graphics/apis/vulkan/framebuffer.h>
#include <atta/graphics/apis/vulkan/image.h>
#include <atta/graphics/apis/vulkan/renderQueue.h>
#include <atta/graphics/apis/vulkan/vulkanAPI.h>
#include <atta/graphics/interface.h>

namespace atta::graphics::vk {

RenderPass::RenderPass(const graphics::RenderPass::CreateInfo& info) : graphics::RenderPass(info), _device(common::getDevice()) {
    std::vector<VkAttachmentDescription> attachments;
    for (std::shared_ptr<gfx::Image> image : _framebuffer->getImages()) {
        Image::Format format = image->getFormat();
        if (Image::isColorFormat(format)) {
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = vk::Image::convertFormat(format);
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // TODO _device->getMsaaSamples();
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            attachments.push_back(colorAttachment);
        } else if (Image::isDepthFormat(format) || Image::isStencilFormat(format)) {
            VkAttachmentDescription depthAttachment{};
            depthAttachment.format = vk::Image::convertFormat(format);
            depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attachments.push_back(depthAttachment);
        }
    }

    // Subpass
    VkAttachmentReference colorAttachmentRef{};
    VkAttachmentReference depthStencilAttachmentRef{};
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    if (_framebuffer->hasColorAttachment()) {
        subpass.colorAttachmentCount = 1;
        colorAttachmentRef.attachment = _framebuffer->getColorAttachmentIndex();
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        subpass.pColorAttachments = &colorAttachmentRef;
    }
    if (_framebuffer->hasDepthAttachment()) {
        depthStencilAttachmentRef.attachment = _framebuffer->getDepthAttachmentIndex();
        depthStencilAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        subpass.pDepthStencilAttachment = &depthStencilAttachmentRef;
    }

    // Subpass dependency
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    if (_framebuffer->hasDepthAttachment()) {
        dependency.srcStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }

    // Create render pass
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

void RenderPass::begin(std::shared_ptr<gfx::RenderQueue> renderQueue) {
    _renderQueue = renderQueue;
    begin(std::dynamic_pointer_cast<vk::RenderQueue>(_renderQueue)->getCommandBuffer());
}

void RenderPass::end() {
    end(std::dynamic_pointer_cast<vk::RenderQueue>(_renderQueue)->getCommandBuffer());
    _renderQueue.reset();
}

void RenderPass::begin(VkCommandBuffer commandBuffer) {
    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = _renderPass;
    renderPassBeginInfo.framebuffer = std::dynamic_pointer_cast<vk::Framebuffer>(_framebuffer)->getHandle();
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = {_framebuffer->getWidth(), _framebuffer->getHeight()};
    // LOG_DEBUG("RenderPass", "Frame buf is $2 -> $0 $1", _framebuffer->getWidth(), _framebuffer->getHeight(), _framebuffer->getDebugName());

    std::vector<VkClearValue> clearValues{};
    for (std::shared_ptr<gfx::Image> image : _framebuffer->getImages()) {
        Image::Format format = image->getFormat();
        if (Image::isColorFormat(format)) {
            vec4 color = _framebuffer->getClearColor();
            VkClearValue clearColor = {{color.x, color.y, color.z, color.w}};
            clearValues.push_back(clearColor);
        } else if (Image::isDepthFormat(format)) {
            VkClearValue clearDepth = {1.0f, 0};
            clearValues.push_back(clearDepth);
        }
    }
    renderPassBeginInfo.clearValueCount = clearValues.size();
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::end(VkCommandBuffer commandBuffer) { vkCmdEndRenderPass(commandBuffer); }

VkRenderPass RenderPass::getHandle() const { return _renderPass; }

void RenderPass::setFramebuffer(std::shared_ptr<Framebuffer> framebuffer) { _framebuffer = framebuffer; }

} // namespace atta::graphics::vk
