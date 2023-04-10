//--------------------------------------------------
// Atta Graphics Module
// pipelineLayout.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/pipelineLayout.h>

namespace atta::graphics::vk {

PipelineLayout::PipelineLayout(std::shared_ptr<DescriptorSetLayout> descriptorSetLayout)
    : _device(common::getDevice()), _descriptorSetLayout(descriptorSetLayout) {
    VkDescriptorSetLayout descriptorSetLayouts[] = {_descriptorSetLayout->getHandle()};

    VkPipelineLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = 1;
    info.pSetLayouts = descriptorSetLayouts;
    info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(_device->getHandle(), &info, nullptr, &_pipelineLayout) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::PipelineLayout", "Failed to create pipeline layout!");
}

PipelineLayout::~PipelineLayout() {
    if (_pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(_device->getHandle(), _pipelineLayout, nullptr);
}

VkPipelineLayout PipelineLayout::getHandle() const { return _pipelineLayout; }

} // namespace atta::graphics::vk
