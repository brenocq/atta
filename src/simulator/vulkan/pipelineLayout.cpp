//--------------------------------------------------
// Robot Simulator
// pipelineLayout.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "pipelineLayout.h"

PipelineLayout::PipelineLayout(Device* device)
{
	_device = device;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(_device->handle(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		std::cout << BOLDRED << "[PipelineLayout]" << RESET << RED << " Failed to create pipeline layout!" << RESET << std::endl;
		exit(1);
	}
}

PipelineLayout::~PipelineLayout()
{
	if(_pipelineLayout != nullptr)
	{
		vkDestroyPipelineLayout(_device->handle(), _pipelineLayout, nullptr);
		_pipelineLayout = nullptr;
	}
}
