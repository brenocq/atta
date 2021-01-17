//--------------------------------------------------
// Robot Simulator
// descriptorBinding.h
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_DESCRIPTOR_BINDING_H
#define ATTA_VK_DESCRIPTOR_BINDING_H
#include <cstdint>
#include "vulkan.h"


struct DescriptorBinding
{
	uint32_t binding;// Slot to which the descriptor will be bound, corresponding to the layout index in the shader
	uint32_t descriptorCount;// Number of descriptors to bind
	VkDescriptorType type;// Type of the bound descriptor(s)
	VkShaderStageFlags stage;// Shader stage at which the bound resources will be available
};
#endif// ATTA_VK_DESCRIPTOR_BINDING_H
