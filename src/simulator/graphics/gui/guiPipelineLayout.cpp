//--------------------------------------------------
// Robot Simulator
// guiPipelineLayout.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "guiPipelineLayout.h"
#include "guiStructs.h"
#include "simulator/helpers/log.h"

GuiPipelineLayout::GuiPipelineLayout(std::shared_ptr<Device> device, DescriptorSetLayout* descriptorSetLayout):
	_device(device), _descriptorSetLayout(descriptorSetLayout)
{
	VkDescriptorSetLayout setLayouts[] = { descriptorSetLayout->handle() };
	
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = setLayouts;

	// Push constants
	VkPushConstantRange pushConstantRange;
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(GuiObjectInfo);

	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if(vkCreatePipelineLayout(_device->handle(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
	{
		Log::error("GuiPipelineLayout", "Failed to create gui pipeline layout!");
		exit(1);
	}
}

GuiPipelineLayout::~GuiPipelineLayout()
{
	if(_pipelineLayout != nullptr)
	{
		vkDestroyPipelineLayout(_device->handle(), _pipelineLayout, nullptr);
		_pipelineLayout = nullptr;
	}
}
