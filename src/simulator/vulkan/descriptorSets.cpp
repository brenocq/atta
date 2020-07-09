//--------------------------------------------------
// Robot Simulator
// descriptorSets.cpp
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "descriptorSets.h"
#include "helpers.h"

DescriptorSets::DescriptorSets(Device* device, DescriptorPool* descriptorPool, DescriptorSetLayout* descriptorSetLayout, std::vector<UniformBuffer*> uniformBuffers, Texture* texture)
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

	for(size_t i = 0; i < size; i++) 
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _uniformBuffers[i]->handle();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture->getImageView()->handle();
		imageInfo.sampler = texture->getSampler()->handle();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = _descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = _descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(_device->handle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

DescriptorSets::~DescriptorSets()
{
}
