//--------------------------------------------------
// Robot Simulator
// pipeline.h
// Date: 2020-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PIPELINE_H
#define PIPELINE_H

#include "../vulkan.h"
#include <iostream>
#include <vector>
#include <string>

#include "../device.h"
#include "../shaderModule.h"
#include "../swapChain.h"
#include "../pipelineLayout.h"
#include "../renderPass.h"
#include "../descriptorSetManager.h"
#include "../descriptorBinding.h"
#include "../vertex.h"
#include "../depthBuffer.h"
#include "../colorBuffer.h"
#include "../../scene.h"

class Pipeline
{
	public:
	Pipeline(Device* device, 
			SwapChain* swapChain, 
			DepthBuffer* depthBuffer, 
			ColorBuffer* colorBuffer, 
			std::vector<UniformBuffer*> uniformBuffers, 
			Scene* scene);
	~Pipeline();

	VkPipeline handle() const { return _graphicsPipeline; }
	RenderPass* getRenderPass() const { return _renderPass; }
	PipelineLayout* getPipelineLayout() const { return _pipelineLayout; }
	DescriptorSetManager* getDescriptorSetManager() const { return _descriptorSetManager; }
	DescriptorSets* getDescriptorSets() const { return _descriptorSetManager->getDescriptorSets(); }

	private:
	VkPipeline _graphicsPipeline;

	Device* _device;
	SwapChain* _swapChain;
	DepthBuffer* _depthBuffer;
	ColorBuffer* _colorBuffer;
	ShaderModule* _vertShaderModule;
	ShaderModule* _fragShaderModule;
	PipelineLayout* _pipelineLayout;
	RenderPass* _renderPass;
	Scene* _scene;
	DescriptorSetManager* _descriptorSetManager;
};

#endif// GRAPHICS_PIPELINE_H
