//--------------------------------------------------
// Atta Graphics Module
// fence.h
// Date: 2023-04-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_FENCE_H
#define ATTA_GRAPHICS_APIS_VULKAN_FENCE_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class Fence {
  public:
    Fence(std::shared_ptr<Device> device);
    ~Fence();

    VkFence getHandle();
    std::shared_ptr<Device> getDevice() const;

    void reset();
    void wait(uint64_t timeout) const;

  private:
    VkFence _fence;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_FENCE_H
