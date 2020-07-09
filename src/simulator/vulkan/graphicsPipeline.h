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
#include "descriptorSetLayout.h"
#include "vertex.h"
#include "depthBuffer.h"
#include "colorBuffer.h"

class GraphicsPipeline
{
	public:
	GraphicsPipeline(Device* device, SwapChain* swapChain, DepthBuffer* depthBuffer, ColorBuffer* colorBuffer, DescriptorSetLayout* descriptorSetLayout);
	~GraphicsPipeline();

	VkPipeline handle() const { return _graphicsPipeline; }
	RenderPass* getRenderPass() const { return _renderPass; }
	PipelineLayout* getPipelineLayout() const { return _pipelineLayout; }

	private:
	VkPipeline _graphicsPipeline;

	Device* _device;
	SwapChain* _swapChain;
	DepthBuffer* _depthBuffer;
	ColorBuffer* _colorBuffer;
	ShaderModule* _vertShaderModule;
	ShaderModule* _fragShaderModule;
	DescriptorSetLayout* _descriptorSetLayout;
	PipelineLayout* _pipelineLayout;
	RenderPass* _renderPass;
};

#endif// GRAPHICS_PIPELINE_H
