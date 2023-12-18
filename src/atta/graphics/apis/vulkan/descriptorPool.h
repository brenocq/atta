//--------------------------------------------------
// Atta Graphics Module
// descriptorPool.h
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_POOL_H
#define ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_POOL_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/descriptorSetLayout.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class DescriptorPool {
  public:
    DescriptorPool(std::vector<VkDescriptorPoolSize> poolSizes, size_t maxSets);
    ~DescriptorPool();

    VkDescriptorPool getHandle() const;
    std::shared_ptr<Device> getDevice() const;

  private:
    VkDescriptorPool _descriptorPool;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_POOL_H
