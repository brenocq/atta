//--------------------------------------------------
// Atta Graphics Module
// descriptorPool.cpp
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/descriptorPool.h>

namespace atta::graphics::vk {

DescriptorPool::DescriptorPool(std::vector<DescriptorSetLayout::Binding> descriptorBindings, size_t maxSets) : _device(common::getDevice()) {
    std::vector<VkDescriptorPoolSize> poolSizes;
    for (auto binding : descriptorBindings)
        poolSizes.push_back(VkDescriptorPoolSize{binding.type, static_cast<uint32_t>(binding.descriptorCount * maxSets)});

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(maxSets);

    if (vkCreateDescriptorPool(_device->getHandle(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::DescriptorPool", "Failed to create descriptor pool");
}

DescriptorPool::~DescriptorPool() {
    if (_descriptorPool != VK_NULL_HANDLE)
        vkDestroyDescriptorPool(_device->getHandle(), _descriptorPool, nullptr);
}

VkDescriptorPool DescriptorPool::getHandle() const { return _descriptorPool; }

std::shared_ptr<Device> DescriptorPool::getDevice() const { return _device; }

} // namespace atta::graphics::vk