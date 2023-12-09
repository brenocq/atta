//--------------------------------------------------
// Atta Graphics Module
// framebuffer.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/framebuffer.h>

namespace atta::graphics::vk {

Framebuffer::Framebuffer(const gfx::Framebuffer::CreateInfo& info)
    : gfx::Framebuffer(info), _framebuffer(VK_NULL_HANDLE), _device(common::getDevice()) {
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

Framebuffer::~Framebuffer() {
    if (_framebuffer != VK_NULL_HANDLE)
        vkDestroyFramebuffer(_device->getHandle(), _framebuffer, nullptr);
}

void Framebuffer::bind(bool clear) {
    if (_framebuffer == VK_NULL_HANDLE) {
        LOG_WARN("gfx::vk::Framebuffer", "Trying to bind framebuffer [w]$0[] that was never created", _debugName);
    }
}
void Framebuffer::unbind() {}

void Framebuffer::resize(uint32_t width, uint32_t height, bool forceRecreate) {}

int Framebuffer::readPixel(unsigned attachmentIndex, unsigned x, unsigned y) {}
std::vector<uint8_t> Framebuffer::readImage(unsigned attachmentIndex) {}

VkFramebuffer Framebuffer::getHandle() const { return _framebuffer; }
std::shared_ptr<Device> Framebuffer::getDevice() const { return _device; }
std::shared_ptr<RenderPass> Framebuffer::getRenderPass() const { return _renderPass; }

void Framebuffer::create(std::shared_ptr<RenderPass> renderPass) {
    _renderPass = renderPass;

    // Get image views
    std::vector<VkImageView> attachments;
    for (std::shared_ptr<gfx::Image> image : _images)
        attachments.push_back(std::dynamic_pointer_cast<vk::Image>(image)->getImageViewHandle());

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
        LOG_ERROR("gfx::vk::FrameBuffer", "Failed to create frame buffer!");
}

} // namespace atta::graphics::vk
