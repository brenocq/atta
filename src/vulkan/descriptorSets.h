#ifndef DESCRIPTOR_SETS_H
#define DESCRIPTOR_SETS_H

#include <map>
#include <vector>
#include "buffer.h"
#include "descriptorPool.h"
#include "descriptorSetLayout.h"
#include "imageView.h"

class DescriptorSets
{
	public:
		DescriptorSets(
			DescriptorPool* descriptorPool, 
			DescriptorSetLayout* layout,
		    std::map<uint32_t, VkDescriptorType> bindingTypes, 
			size_t size);
		~DescriptorSets();

		VkDescriptorSet handle(uint32_t index) const { return _descriptorSets[index]; }

		VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, uint32_t count = 1) const;
		VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkDescriptorImageInfo& imageInfo, uint32_t count = 1) const;
		VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkWriteDescriptorSetAccelerationStructureNV& structureInfo, uint32_t count = 1) const;

		void updateDescriptors(uint32_t index, const std::vector<VkWriteDescriptorSet>& descriptorWrites);

	private:
		VkDescriptorType getBindingType(uint32_t binding) const;

		DescriptorPool* _descriptorPool;
		const std::map<uint32_t, VkDescriptorType> _bindingTypes;
		std::vector<VkDescriptorSet> _descriptorSets;
};

#endif// DESCRIPTOR_SETS_H
