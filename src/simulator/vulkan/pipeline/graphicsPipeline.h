//--------------------------------------------------
// Robot Simulator
// graphicsPipeline.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include <iostream>
#include <vector>
#include <string.h>

#include "pipeline.h"
#include "simulator/vulkan/renderPass.h"
#include "simulator/vulkan/colorBuffer.h"
#include "simulator/vulkan/depthBuffer.h"

class GraphicsPipeline : public Pipeline
{
	public:
		// Graphics Pipeline with swapchain
		GraphicsPipeline(Device* device, 
				SwapChain* swapChain, 
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		// Offline Graphics Pipeline
		GraphicsPipeline(Device* device, 
				VkExtent2D extent, VkFormat format,
				ImageView* imageView,
				UniformBuffer* uniformBuffer, 
				Scene* scene);
		// Base constructor
		GraphicsPipeline(Device* device, 
				VkExtent2D extent, VkFormat format,
				std::vector<ImageView*> imageViews, 
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		~GraphicsPipeline();

		void beginRender(VkCommandBuffer commandBuffer, int imageIndex=0);
		void endRender(VkCommandBuffer commandBuffer);
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

#endif// GRAPHICS_PIPELINE_H
