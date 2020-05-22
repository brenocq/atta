#include "descriptorSets.h"
#include <array>
#include <utility>

DescriptorSets::DescriptorSets(
	DescriptorPool* descriptorPool, 
	DescriptorSetLayout* layout,
	std::map<uint32_t, VkDescriptorType> bindingTypes,
	const size_t size):
	_descriptorPool(descriptorPool),
	_bindingTypes(std::move(bindingTypes))
{
	std::vector<VkDescriptorSetLayout> layouts(size, layout->handle());

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool->handle();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
	allocInfo.pSetLayouts = layouts.data();

	_descriptorSets.resize(size);

	if(vkAllocateDescriptorSets(descriptorPool->device()->handle(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[DescriptorSets] Failed to allocate descriptor sets!" << RESET << std::endl;
		exit(1);
	}
}

DescriptorSets::~DescriptorSets()
{
	//if (!descriptorSets_.empty())
	//{
	//	vkFreeDescriptorSets(
	//		descriptorPool_.Device().Handle(),
	//		descriptorPool_.Handle(),
	//		static_cast<uint32_t>(descriptorSets_.size()),
	//		descriptorSets_.data());

	//	descriptorSets_.clear();
	//}
}

VkWriteDescriptorSet DescriptorSets::bind(const uint32_t index, const uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, const uint32_t count) const
{
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = _descriptorSets[index];
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = getBindingType(binding);
	descriptorWrite.descriptorCount = count;
	descriptorWrite.pBufferInfo = &bufferInfo;

	return descriptorWrite;
}

VkWriteDescriptorSet DescriptorSets::bind(const uint32_t index, const uint32_t binding, const VkDescriptorImageInfo& imageInfo, const uint32_t count) const
{
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = _descriptorSets[index];
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = getBindingType(binding);
	descriptorWrite.descriptorCount = count;
	descriptorWrite.pImageInfo = &imageInfo;

	return descriptorWrite;
}

VkWriteDescriptorSet DescriptorSets::bind(uint32_t index, uint32_t binding, const VkWriteDescriptorSetAccelerationStructureNV& structureInfo, const uint32_t count) const
{
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = _descriptorSets[index];
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = getBindingType(binding);
	descriptorWrite.descriptorCount = count;
	descriptorWrite.pNext = &structureInfo;

	return descriptorWrite;
}

void DescriptorSets::updateDescriptors(uint32_t index, const std::vector<VkWriteDescriptorSet>& descriptorWrites)
{
	vkUpdateDescriptorSets(
		_descriptorPool->device()->handle(),
		static_cast<uint32_t>(descriptorWrites.size()),
		descriptorWrites.data(), 0, nullptr);
}

VkDescriptorType DescriptorSets::getBindingType(uint32_t binding) const
{
	const auto it = _bindingTypes.find(binding);
	if (it == _bindingTypes.end())
	{
		std::cout << BOLDRED << "[DescriptorSets] Binding not found!" << RESET << std::endl;
		exit(1);
	}

	return it->second;
}
