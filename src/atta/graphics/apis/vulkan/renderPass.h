//--------------------------------------------------
// Atta Graphics Module
// renderPass.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_RENDER_PASS_H
#define ATTA_GRAPHICS_APIS_VULKAN_RENDER_PASS_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/renderPass.h>

namespace atta::graphics::vk {

class RenderPass final : public graphics::RenderPass {
  public:
    RenderPass(const graphics::RenderPass::CreateInfo& info, std::shared_ptr<Device> device);
    ~RenderPass();

    VkRenderPass getHandle() const;

  private:
    VkRenderPass _renderPass;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_RENDER_PASS_H
