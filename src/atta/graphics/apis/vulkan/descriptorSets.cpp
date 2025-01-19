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

void DescriptorSets::update(uint32_t bindingIdx, std::shared_ptr<UniformBuffer> uniformBuffer) const {
    DescriptorSetLayout::Binding binding = _descriptorSetLayout->getBindings()[bindingIdx];

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffer->getHandle();
    bufferInfo.offset = 0;
    bufferInfo.range = uniformBuffer->getSize();

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = _descriptorSets[0];
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = binding.type;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    vkUpdateDescriptorSets(_device->getHandle(), 1, &descriptorWrite, 0, nullptr);
}

void DescriptorSets::update(uint32_t bindingIdx, std::shared_ptr<Image> image) const {
    DescriptorSetLayout::Binding binding = _descriptorSetLayout->getBindings()[bindingIdx];

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = image->getImageViewHandle();
    imageInfo.sampler = image->getSamplerHandle();

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = _descriptorSets[0];
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = binding.type;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;
    vkUpdateDescriptorSets(_device->getHandle(), 1, &descriptorWrite, 0, nullptr);
}

void DescriptorSets::bind(VkCommandBuffer commandBuffer, uint32_t setIdx, size_t index, const uint32_t* dynamicOffset) {
    uint32_t dynamicOffsetCount = dynamicOffset == nullptr ? 0 : 1;
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout->getHandle(), setIdx, 1, &_descriptorSets[0],
                            dynamicOffsetCount, dynamicOffset);
}

} // namespace atta::graphics::vk
