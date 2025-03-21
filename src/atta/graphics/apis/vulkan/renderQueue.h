//--------------------------------------------------
// Atta Graphics Module
// renderQueue.h
// Date: 2023-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_RENDER_QUEUE_H
#define ATTA_GRAPHICS_APIS_VULKAN_RENDER_QUEUE_H

#include <atta/graphics/apis/vulkan/commandBuffers.h>
#include <atta/graphics/apis/vulkan/commandPool.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/fence.h>
#include <atta/graphics/renderQueue.h>

namespace atta::graphics::vk {

class RenderQueue final : public gfx::RenderQueue {
  public:
    RenderQueue();
    ~RenderQueue();

    void begin() override;
    void end() override;

    VkCommandBuffer getCommandBuffer();

  private:
    std::shared_ptr<vk::Device> _device;
    std::shared_ptr<vk::CommandPool> _commandPool;
    std::shared_ptr<vk::CommandBuffers> _commandBuffers;
    std::shared_ptr<vk::Fence> _fence;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_RENDER_QUEUE_H
