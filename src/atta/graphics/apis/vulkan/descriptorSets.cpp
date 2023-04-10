//--------------------------------------------------
// Atta Graphics Module
// descriptorSets.cpp
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/descriptorSets.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

DescriptorSets::DescriptorSets(std::shared_ptr<DescriptorPool> descriptorPool, std::shared_ptr<DescriptorSetLayout> descriptorSetLayout,
                               std::shared_ptr<PipelineLayout> pipelineLayout, size_t size)
    : _device(common::getDevice()), _descriptorPool(descriptorPool), _descriptorSetLayout(descriptorSetLayout), _pipelineLayout(pipelineLayout) {
    std::vector<VkDescriptorSetLayout> layouts(size, _descriptorSetLayout->getHandle());

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _descriptorPool->getHandle();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
    allocInfo.pSetLayouts = layouts.data();

    _descriptorSets.resize(size);

    if (vkAllocateDescriptorSets(_device->getHandle(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::DescriptorSets", "Failed to allocate descriptor sets!");
}

DescriptorSets::~DescriptorSets() {}

std::vector<VkDescriptorSet> DescriptorSets::getHandle() const { return _descriptorSets; }

std::shared_ptr<Device> DescriptorSets::getDevice() const { return _device; }

VkWriteDescriptorSet DescriptorSets::update(uint32_t index, std::shared_ptr<UniformBuffer> uniformBuffer) const {
    DescriptorSetLayout::Binding binding = _descriptorSetLayout->getBindings()[index];

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffer->getHandle();
    bufferInfo.offset = 0;
    bufferInfo.range = uniformBuffer->getSize();

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = _descriptorSets[index];
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = binding.type;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    vkUpdateDescriptorSets(_device->getHandle(), 1, &descriptorWrite, 0, nullptr);
}

void DescriptorSets::bind(size_t index) {
    vkCmdBindDescriptorSets(common::getCommandBuffers()->getCurrent(), VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout->getHandle(), 0, 1,
                            &_descriptorSets[index], 0, nullptr);
}

} // namespace atta::graphics::vk
