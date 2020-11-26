//--------------------------------------------------
// Robot Simulator
// maskPipeline.h
// Date: 26/11/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MASK_PIPELINE_H
#define MASK_PIPELINE_H

#include <iostream>
#include <vector>
#include <string.h>

#include "pipeline.h"
#include "simulator/vulkan/renderPass.h"
#include "simulator/vulkan/colorBuffer.h"
#include "simulator/vulkan/depthBuffer.h"

class MaskPipeline : public Pipeline
{
	public:
		// Graphics Pipeline with swapchain
		MaskPipeline(Device* device, 
				SwapChain* swapChain, 
				RenderPass* renderPass,
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		// Offline Graphics Pipeline
		MaskPipeline(Device* device, 
				RenderPass* renderPass,
				VkExtent2D extent, VkFormat format,
				ImageView* imageView,
				UniformBuffer* uniformBuffer, 
				Scene* scene);
		// Base constructor
		MaskPipeline(Device* device, 
				RenderPass* renderPass,
				VkExtent2D extent, VkFormat format,
				std::vector<ImageView*> imageViews, 
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		~MaskPipeline();

		void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		//---------- Getters and Setters ----------//
		DepthBuffer* getDepthBuffer() const { return _depthBuffer; }
		ColorBuffer* getColorBuffer() const { return _colorBuffer; }

	private:
		ColorBuffer* _colorBuffer;
		DepthBuffer* _depthBuffer;

		// Output image info
		VkExtent2D _imageExtent;
		VkFormat _imageFormat;
};

#endif// MASK_PIPELINE_H
