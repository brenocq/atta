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
	~DescriptorSets();

	std::vector<VkDescriptorSet> handle() const { return _descriptorSets; }
	Device* getDevice() const { return _device; }

	private:
	std::vector<VkDescriptorSet> _descriptorSets;
	Device* _device;
	DescriptorPool* _descriptorPool;
	DescriptorSetLayout* _descriptorSetLayout;
	std::vector<UniformBuffer*> _uniformBuffers;
};

#endif// DESCRIPTOR_SET_H
