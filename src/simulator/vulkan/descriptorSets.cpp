//--------------------------------------------------
// Robot Simulator
// descriptorSets.cpp
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "descriptorSets.h"
#include "helpers.h"

DescriptorSets::DescriptorSets(Device* device, DescriptorPool* descriptorPool, DescriptorSetLayout* descriptorSetLayout, std::vector<UniformBuffer*> uniformBuffers)
{
	_device = device;
	_descriptorPool = descriptorPool;
	_descriptorSetLayout = descriptorSetLayout;
	_uniformBuffers = uniformBuffers;
	int size = uniformBuffers.size();

	std::vector<VkDescriptorSetLayout> layouts(size, _descriptorSetLayout->handle());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool->handle();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
	allocInfo.pSetLayouts = layouts.data();

	_descriptorSets.resize(size);
	if(vkAllocateDescriptorSets(_device->handle(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[DescriptorSets]" << RESET << RED << " Failed to allocate descriptor sets!" << RESET << std::endl;
		exit(1);
	}

	for (size_t i = 0; i < size; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _uniformBuffers[i]->handle();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = _descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;

		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;

		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional
		vkUpdateDescriptorSets(_device->handle(), 1, &descriptorWrite, 0, nullptr);
	}
}

DescriptorSets::~DescriptorSets()
{
}
