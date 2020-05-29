#include "descriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(Device* device, const std::vector<DescriptorBinding>& descriptorBindings):
	_device(device)
{
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

	for (const auto& binding : descriptorBindings)
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

	if(vkCreateDescriptorSetLayout(device->handle(), &layoutInfo, nullptr, &_layout) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[DescriptorSetLayout] Failed to create descriptor set layout!" << RESET << std::endl;
		exit(1);
	}
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	if (_layout != nullptr)
		vkDestroyDescriptorSetLayout(_device->handle(), _layout, nullptr);
}
