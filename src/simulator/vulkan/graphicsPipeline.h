//--------------------------------------------------
// Robot Simulator
// graphicsPipeline.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string.h>

#include "device.h"
#include "shaderModule.h"
#include "swapChain.h"
#include "pipelineLayout.h"
#include "renderPass.h"

class GraphicsPipeline
{
	public:
	GraphicsPipeline(Device* device, SwapChain* swapChain);
	~GraphicsPipeline();

	VkPipeline handle() const { return _graphicsPipeline; }
	RenderPass* getRenderPass() const { return _renderPass; }

	private:
	VkPipeline _graphicsPipeline;

	Device* _device;
	SwapChain* _swapChain;
	ShaderModule* _vertShaderModule;
	ShaderModule* _fragShaderModule;
	PipelineLayout* _pipelineLayout;
	RenderPass* _renderPass;
};

#endif// GRAPHICS_PIPELINE_H
