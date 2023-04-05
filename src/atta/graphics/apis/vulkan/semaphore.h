//--------------------------------------------------
// Atta Graphics Module
// semaphore.h
// Date: 2023-04-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_SEMAPHORE_H
#define ATTA_GRAPHICS_APIS_VULKAN_SEMAPHORE_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class Semaphore {
  public:
    Semaphore(std::shared_ptr<Device> device);
    ~Semaphore();

    VkSemaphore getHandle() const;
    std::shared_ptr<Device> getDevice() const;

  private:
    VkSemaphore _semaphore;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SEMAPHORE_H
