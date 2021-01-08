//--------------------------------------------------
// Robot Simulator
// outlinePipeline.h
// Date: 2020-11-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef OUTLINE_PIPELINE_H
#define OUTLINE_PIPELINE_H

#include <iostream>
#include <vector>
#include <string.h>

#include "pipeline.h"
#include "simulator/graphics/vulkan/renderPass.h"
#include "simulator/graphics/vulkan/colorBuffer.h"
#include "simulator/graphics/vulkan/depthBuffer.h"

class OutlinePipeline : public Pipeline
{
	public:
		// Graphics Pipeline with swapchain
		OutlinePipeline(std::shared_ptr<Device> device, 
				std::shared_ptr<SwapChain> swapChain, 
				RenderPass* renderPass,
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		// Offline Graphics Pipeline
		OutlinePipeline(std::shared_ptr<Device> device, 
				RenderPass* renderPass,
				VkExtent2D extent, VkFormat format,
				ImageView* imageView,
				UniformBuffer* uniformBuffer, 
				Scene* scene);
		// Base constructor
		OutlinePipeline(std::shared_ptr<Device> device, 
				RenderPass* renderPass,
				VkExtent2D extent, VkFormat format,
				std::vector<ImageView*> imageViews, 
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		~OutlinePipeline();

		void render(VkCommandBuffer commandBuffer, int imageIndex=0);

	private:
		// Output image info
		VkExtent2D _imageExtent;
		VkFormat _imageFormat;
};

#endif// OUTLINE_PIPELINE_H
