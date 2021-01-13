//--------------------------------------------------
// Robot Simulator
// pipelineLayout.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "pipelineLayout.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	PipelineLayout::PipelineLayout(std::shared_ptr<Device> device, std::shared_ptr<DescriptorSetLayout> descriptorSetLayout):
		_device(device), _descriptorSetLayout(descriptorSetLayout)
	{
		VkDescriptorSetLayout setLayouts[] = { descriptorSetLayout->handle() };
		
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = setLayouts;

		// Push constants
		VkPushConstantRange pushConstantRange;
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(ObjectInfo);

		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if(vkCreatePipelineLayout(_device->handle(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
		{
			Log::error("PipelineLayout", "Failed to create pipeline layout!");
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
}
