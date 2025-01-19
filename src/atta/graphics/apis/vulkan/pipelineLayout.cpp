//--------------------------------------------------
// Atta Graphics Module
// pipelineLayout.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/pipelineLayout.h>

namespace atta::graphics::vk {

PipelineLayout::PipelineLayout(std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts, std::shared_ptr<PushConstant> pushConstant)
    : _device(common::getDevice()), _descriptorSetLayouts(descriptorSetLayouts) {
    std::vector<VkDescriptorSetLayout> descriptorSetLayoutHandles;
    for (const auto& descSet : descriptorSetLayouts)
        descriptorSetLayoutHandles.push_back(descSet->getHandle());

    VkPipelineLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = descriptorSetLayoutHandles.size();
    info.pSetLayouts = descriptorSetLayoutHandles.data();
    if (pushConstant) {
        info.pushConstantRangeCount = 1;
        info.pPushConstantRanges = &pushConstant->getRange();
    }

    if (vkCreatePipelineLayout(_device->getHandle(), &info, nullptr, &_pipelineLayout) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::PipelineLayout", "Failed to create pipeline layout!");
}

PipelineLayout::~PipelineLayout() {
    if (_pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(_device->getHandle(), _pipelineLayout, nullptr);
}

VkPipelineLayout PipelineLayout::getHandle() const { return _pipelineLayout; }

} // namespace atta::graphics::vk
