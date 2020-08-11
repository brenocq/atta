//--------------------------------------------------
// Robot Simulator
// pipeline.h
// Date: 2020-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PIPELINE_H
#define PIPELINE_H

#include "../vulkan.h"
#include <iostream>
#include <vector>
#include <string>

#include "pipelineLayout.h"
#include "../device.h"
#include "../shaderModule.h"
#include "../swapChain.h"
#include "../renderPass.h"
#include "../descriptorSetManager.h"
#include "../descriptorBinding.h"
#include "../vertex.h"
#include "../../scene.h"

class Pipeline
{
	public:
		Pipeline(Device* device, 
				SwapChain* swapChain, 
				RenderPass* renderPass,
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		~Pipeline();

		VkPipeline handle() const { return _pipeline; }
		PipelineLayout* getPipelineLayout() const { return _pipelineLayout; }
		DescriptorSetManager* getDescriptorSetManager() const { return _descriptorSetManager; }
		DescriptorSets* getDescriptorSets() const { return _descriptorSetManager->getDescriptorSets(); }

	protected:
		VkPipeline _pipeline;
		Device* _device;
		SwapChain* _swapChain;
		RenderPass* _renderPass;
		Scene* _scene;

		PipelineLayout* _pipelineLayout;
		DescriptorSetManager* _descriptorSetManager;
		ShaderModule* _vertShaderModule;
		ShaderModule* _fragShaderModule;
};

#endif// PIPELINE_H
