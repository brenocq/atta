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
#include "../imageView.h"
#include "../frameBuffer.h"
#include "../descriptorSetManager.h"
#include "../descriptorBinding.h"
#include "../vertex.h"
#include "../../scene.h"

class Pipeline
{
	public:
		Pipeline(Device* device, 
				std::vector<ImageView*> imageViews,
				Scene* scene);
		virtual ~Pipeline();

		virtual void render(VkCommandBuffer commandBuffer, int imageIndex=0) = 0;

		//---------- Getters and Setters ----------//
		VkPipeline handle() const { return _pipeline; }
		PipelineLayout* getPipelineLayout() const { return _pipelineLayout; }
		DescriptorSetManager* getDescriptorSetManager() const { return _descriptorSetManager; }
		DescriptorSets* getDescriptorSets() const { return _descriptorSetManager->getDescriptorSets(); }
		std::vector<ImageView*> getImageViews() const { return _imageViews; }
		std::vector<FrameBuffer*> getFrameBuffers() const { return _frameBuffers; }
		RenderPass* getRenderPass() const { return _renderPass; }

	protected:
		VkPipeline _pipeline;
		Device* _device;
		std::vector<ImageView*> _imageViews;
		std::vector<FrameBuffer*> _frameBuffers;
		RenderPass* _renderPass;
		Scene* _scene;

		PipelineLayout* _pipelineLayout;
		DescriptorSetManager* _descriptorSetManager;
		ShaderModule* _vertShaderModule;
		ShaderModule* _fragShaderModule;
};

#endif// PIPELINE_H
