// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/apis/vulkan/framebuffer.h>

namespace atta::graphics::vk {

Framebuffer::Framebuffer(const gfx::Framebuffer::CreateInfo& info)
    : gfx::Framebuffer(info), _framebuffer(VK_NULL_HANDLE), _device(common::getDevice()) {
    resize(_width, _height);
}

Framebuffer::~Framebuffer() {
    if (_framebuffer != VK_NULL_HANDLE)
        vkDestroyFramebuffer(_device->getHandle(), _framebuffer, nullptr);
}

void Framebuffer::bind(bool clear) {
    if (_framebuffer == VK_NULL_HANDLE) {
        LOG_WARN("gfx::vk::Framebuffer", "Trying to bind framebuffer [w]$0[] that was never created", _debugName);
    }
}
void Framebuffer::unbind() {
    VkImageLayout layout = _isSwapchain ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    for (size_t i = 0; i < _images.size(); i++)
        std::dynamic_pointer_cast<vk::Image>(_images[i])->setLayout(layout);
}

void Framebuffer::resize(uint32_t width, uint32_t height, bool forceRecreate) {
    _width = width;
    _height = height;

    // Create attachment images
    _images.clear();
    for (unsigned i = 0; i < _attachments.size(); i++) {
        std::shared_ptr<vk::Image> image;
        if (_attachments[i].image) {
            // If already created
            image = std::dynamic_pointer_cast<vk::Image>(_attachments[i].image);
        } else {
            // If create from format
            gfx::Image::CreateInfo info;
            info.format = _attachments[i].format;
            info.width = _width;
            info.height = _height;
            image = std::make_shared<vk::Image>(info);
        }
        _images.push_back(std::dynamic_pointer_cast<gfx::Image>(image));
    }
}

void Framebuffer::setLayer(uint32_t layer) {
    // For now, support only framebuffers with a single attachment that is a cubemap
    if (_images.size() != 1) {
        LOG_ERROR("gfx::vk::Framebuffer", "setLayer is only supported for framebuffers with one attachment, not for framebuffer [w]$0[]", _debugName);
        return;
    }

    // Get the image and ensure it's a cubemap
    std::shared_ptr<gfx::Image> image = _images[0];
    std::shared_ptr<vk::Image> vkImage = std::dynamic_pointer_cast<vk::Image>(image);
    if (!vkImage->isCubemap()) {
        LOG_ERROR("gfx::vk::Framebuffer", "setLayer called on a non-cubemap image for framebuffer [w]$0[]", _debugName);
        return;
    }

    // Destroy the existing framebuffer if it exists
    if (_framebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(_device->getHandle(), _framebuffer, nullptr);
        _framebuffer = VK_NULL_HANDLE;
    }

    // Retrieve the image view for the desired cubemap face (layer)
    VkImageView faceView = vkImage->getCubemapImageViewHandle(layer);
    if (faceView == VK_NULL_HANDLE) {
        LOG_ERROR("gfx::vk::Framebuffer", "Failed to get cubemap image view for layer %d", layer);
        return;
    }

    // Create a new framebuffer with this per-face view
    std::vector<VkImageView> attachments = {faceView};

    VkFramebufferCreateInfo fbInfo{};
    fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.renderPass = _renderPass->getHandle();
    fbInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    fbInfo.pAttachments = attachments.data();
    fbInfo.width = _width;
    fbInfo.height = _height;
    fbInfo.layers = 1;

    if (vkCreateFramebuffer(_device->getHandle(), &fbInfo, nullptr, &_framebuffer) != VK_SUCCESS) {
        LOG_ERROR("gfx::vk::Framebuffer", "Failed to create framebuffer for cubemap layer %d", layer);
        return;
    }
}

void Framebuffer::setLayerAndMip(uint32_t layer, uint32_t mipLevel) {
    if (_images.size() != 1) {
        LOG_ERROR("gfx::vk::Framebuffer", "setLayerAndMip is only supported for framebuffers with one attachment, not for framebuffer [w]$0[]", _debugName);
        return;
    }

    std::shared_ptr<vk::Image> vkImage = std::dynamic_pointer_cast<vk::Image>(_images[0]);
    if (!vkImage->isCubemap()) {
        LOG_ERROR("gfx::vk::Framebuffer", "setLayerAndMip called on a non-cubemap image for framebuffer [w]$0[]", _debugName);
        return;
    }

    if (_framebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(_device->getHandle(), _framebuffer, nullptr);
        _framebuffer = VK_NULL_HANDLE;
    }

    VkImageView faceView = vkImage->getCubemapFaceMipImageViewHandle(layer, mipLevel);
    if (faceView == VK_NULL_HANDLE) {
        LOG_ERROR("gfx::vk::Framebuffer", "Failed to get cubemap face+mip image view for layer $0 mip $1", layer, mipLevel);
        return;
    }

    std::vector<VkImageView> attachments = {faceView};

    VkFramebufferCreateInfo fbInfo{};
    fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.renderPass = _renderPass->getHandle();
    fbInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    fbInfo.pAttachments = attachments.data();
    fbInfo.width = _width;
    fbInfo.height = _height;
    fbInfo.layers = 1;

    if (vkCreateFramebuffer(_device->getHandle(), &fbInfo, nullptr, &_framebuffer) != VK_SUCCESS) {
        LOG_ERROR("gfx::vk::Framebuffer", "Failed to create framebuffer for cubemap layer $0 mip $1", layer, mipLevel);
        return;
    }
}

VkFramebuffer Framebuffer::getHandle() const { return _framebuffer; }
std::shared_ptr<Device> Framebuffer::getDevice() const { return _device; }
std::shared_ptr<RenderPass> Framebuffer::getRenderPass() const { return _renderPass; }

void Framebuffer::create(std::shared_ptr<RenderPass> renderPass) {
    _renderPass = renderPass;

    // Destroy framebuffer is necessary
    if (_framebuffer != VK_NULL_HANDLE)
        vkDestroyFramebuffer(_device->getHandle(), _framebuffer, nullptr);

    // Get image views (framebuffer attachments must have levelCount == 1)
    std::vector<VkImageView> attachments;
    for (std::shared_ptr<gfx::Image> image : _images) {
        auto vkImage = std::dynamic_pointer_cast<vk::Image>(image);
        // Cubemap images: use face 0 mip 0 as a valid single-mip placeholder.
        // setLayer() or setLayerAndMip() will replace it with the correct view before rendering.
        if (vkImage->isCubemap())
            attachments.push_back(vkImage->getCubemapFaceMipImageViewHandle(0, 0));
        else
            attachments.push_back(vkImage->getImageViewHandle());
    }

    // Create framebuffer
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = _renderPass->getHandle();
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(_device->getHandle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Framebuffer", "Failed to create frame buffer!");
}

} // namespace atta::graphics::vk
