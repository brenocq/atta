//--------------------------------------------------
// Robot Simulator
// descriptorSets.h
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DESCRIPTOR_SET_H
#define DESCRIPTOR_SET_H

#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include "defines.h"
#include "device.h"
#include "descriptorPool.h"
#include "descriptorSetLayout.h"
#include "uniformBuffer.h"
#include "texture.h"

class DescriptorSets
{
	public:
		DescriptorSets(Device* device, DescriptorPool* descriptorPool, DescriptorSetLayout* descriptorSetLayout, std::vector<UniformBuffer*> uniformBuffers, Texture* texture);
		DescriptorSets(
				Device* device, 
				DescriptorPool* descriptorPool, 
				DescriptorSetLayout* descriptorSetLayout,
				std::map<uint32_t, VkDescriptorType> bindingTypes, 
				size_t size);

		~DescriptorSets();

		std::vector<VkDescriptorSet> handle() const { return _descriptorSets; }
		Device* getDevice() const { return _device; }

		VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, uint32_t count = 1) const;
		VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkDescriptorImageInfo& imageInfo, uint32_t count = 1) const;
		VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkWriteDescriptorSetAccelerationStructureNV& structureInfo, uint32_t count = 1) const;
		void updateDescriptors(uint32_t index, const std::vector<VkWriteDescriptorSet>& descriptorWrites);

	private:
		VkDescriptorType getBindingType(uint32_t binding) const;

		std::vector<VkDescriptorSet> _descriptorSets;
		std::map<uint32_t, VkDescriptorType> _bindingTypes;
		Device* _device;
		DescriptorPool* _descriptorPool;
		DescriptorSetLayout* _descriptorSetLayout;
		std::vector<UniformBuffer*> _uniformBuffers;
};

#endif// DESCRIPTOR_SET_H
