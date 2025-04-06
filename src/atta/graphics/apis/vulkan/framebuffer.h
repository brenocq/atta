//--------------------------------------------------
// Atta Graphics Module
// framebuffer.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_FRAMEBUFFER_H
#define ATTA_GRAPHICS_APIS_VULKAN_FRAMEBUFFER_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/renderPass.h>
#include <atta/graphics/apis/vulkan/image.h>
#include <atta/graphics/framebuffer.h>

namespace atta::graphics::vk {

class Pipeline;
class Framebuffer final : public gfx::Framebuffer {
  public:
    Framebuffer(const gfx::Framebuffer::CreateInfo& info);
    ~Framebuffer();

    void bind(bool clear = true) override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    void setLayer(uint32_t layer) override;

    VkFramebuffer getHandle() const;
    std::shared_ptr<Device> getDevice() const;
    std::shared_ptr<RenderPass> getRenderPass() const;

    void create(std::shared_ptr<RenderPass> renderPass);

  private:
    VkFramebuffer _framebuffer;
    std::shared_ptr<Device> _device;
    std::shared_ptr<RenderPass> _renderPass;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_FRAMEBUFFER_H
