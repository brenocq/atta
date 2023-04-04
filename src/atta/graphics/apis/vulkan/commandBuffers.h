//--------------------------------------------------
// Atta Graphics Module
// commandBuffers.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_COMMAND_BUFFERS_H
#define ATTA_GRAPHICS_APIS_VULKAN_COMMAND_BUFFERS_H

#include <atta/graphics/apis/vulkan/commandPool.h>
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class CommandBuffers {
  public:
    CommandBuffers(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, uint32_t size);
    ~CommandBuffers();

    std::vector<VkCommandBuffer> getHandles() const;
    void setHandles(std::vector<VkCommandBuffer> newCommandBuffers);
    std::shared_ptr<Device> getDevice() const;

    VkCommandBuffer begin(size_t i);
    void end(size_t i);

  private:
    std::vector<VkCommandBuffer> _commandBuffers;
    std::shared_ptr<Device> _device;
    std::shared_ptr<CommandPool> _commandPool;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_COMMAND_BUFFERS_H
