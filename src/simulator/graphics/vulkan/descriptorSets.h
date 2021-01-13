//--------------------------------------------------
// Robot Simulator
// descriptorSets.h
// Date: 2020-07-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_DESCRIPTOR_SET_H
#define ATTA_VK_DESCRIPTOR_SET_H

#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include "defines.h"
#include "device.h"
#include "descriptorPool.h"
#include "descriptorSetLayout.h"
#include "uniformBuffer.h"

namespace atta::vk
{
	class DescriptorSets
	{
		public:
			DescriptorSets(
					std::shared_ptr<Device> device, 
					std::shared_ptr<DescriptorPool> descriptorPool, 
					std::shared_ptr<DescriptorSetLayout> descriptorSetLayout,
					std::map<uint32_t, VkDescriptorType> bindingTypes, 
					size_t size);

			~DescriptorSets();

			std::vector<VkDescriptorSet> handle() const { return _descriptorSets; }
			std::shared_ptr<Device> getDevice() const { return _device; }

			VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkDescriptorBufferInfo& bufferInfo, uint32_t count = 1) const;
			VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkDescriptorImageInfo& imageInfo, uint32_t count = 1) const;
			VkWriteDescriptorSet bind(uint32_t index, uint32_t binding, const VkWriteDescriptorSetAccelerationStructureNV& structureInfo, uint32_t count = 1) const;
			void updateDescriptors(uint32_t index, const std::vector<VkWriteDescriptorSet>& descriptorWrites);

		private:
			VkDescriptorType getBindingType(uint32_t binding) const;

			std::shared_ptr<Device> _device;
			std::shared_ptr<DescriptorPool> _descriptorPool;
			std::shared_ptr<DescriptorSetLayout> _descriptorSetLayout;
			std::vector<VkDescriptorSet> _descriptorSets;
			std::map<uint32_t, VkDescriptorType> _bindingTypes;
			std::vector<std::shared_ptr<UniformBuffer>> _uniformBuffers;
	};
}

#endif// ATTA_VK_DESCRIPTOR_SET_H
