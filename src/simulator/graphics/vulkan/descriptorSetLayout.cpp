//--------------------------------------------------
// Robot Simulator
// descriptorSetLayout.cpp
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "descriptorSetLayout.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device):
		_device(device)
	{

		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();


		if(vkCreateDescriptorSetLayout(_device->handle(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
		{
			Log::error("DescriptorSetLayout", "Failed to create descriptor set layout!");
			exit(1);
		}
	}

	DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<Device> device, std::vector<DescriptorBinding> descriptorBindings):
		_device(device)
	{
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		for (auto binding : descriptorBindings)
		{
			VkDescriptorSetLayoutBinding b = {};
			b.binding = binding.binding;
			b.descriptorCount = binding.descriptorCount;
			b.descriptorType = binding.type;
			b.stageFlags = binding.stage;

			layoutBindings.push_back(b);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();

		if(vkCreateDescriptorSetLayout(_device->handle(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
		{
			std::cout << BOLDRED << "[DescriptorSetLayout]" << RESET << RED << " Failed to create descriptor set layout!" << RESET << std::endl;
			exit(1);
		}
	}


	DescriptorSetLayout::~DescriptorSetLayout()
	{
		if(_descriptorSetLayout != nullptr)
		{
			vkDestroyDescriptorSetLayout(_device->handle(), _descriptorSetLayout, nullptr);
			_descriptorSetLayout = nullptr;

		}
	}
}
