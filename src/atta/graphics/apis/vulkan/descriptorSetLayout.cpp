//--------------------------------------------------
// Atta Graphics Module
// descriptorSetLayout.cpp
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/descriptorSetLayout.h>

namespace atta::graphics::vk {

DescriptorSetLayout::DescriptorSetLayout(std::vector<Binding> bindings) : _device(common::getDevice()), _bindings(bindings) {
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

    for (Binding binding : _bindings) {
        VkDescriptorSetLayoutBinding b{};
        b.binding = binding.binding;
        b.descriptorCount = binding.descriptorCount;
        b.descriptorType = binding.type;
        b.stageFlags = binding.stage;

        layoutBindings.push_back(b);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
    layoutInfo.pBindings = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(_device->getHandle(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::DescriptorSetLayout", "Failed to create descriptor set layout");
}

DescriptorSetLayout::~DescriptorSetLayout() {
    if (_descriptorSetLayout != VK_NULL_HANDLE)
        vkDestroyDescriptorSetLayout(_device->getHandle(), _descriptorSetLayout, nullptr);
}

VkDescriptorSetLayout DescriptorSetLayout::getHandle() const { return _descriptorSetLayout; }

std::shared_ptr<Device> DescriptorSetLayout::getDevice() const { return _device; }

std::vector<DescriptorSetLayout::Binding> DescriptorSetLayout::getBindings() const { return _bindings; }

} // namespace atta::graphics::vk
