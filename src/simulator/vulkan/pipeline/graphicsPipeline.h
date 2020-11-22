//--------------------------------------------------
// Robot Simulator
// graphicsPipeline.h
// Date: 24/06/2020
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
		GraphicsPipeline(Device* device, 
				SwapChain* swapChain, 
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		~GraphicsPipeline();

		void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		//---------- Getters and Setters ----------//
		RenderPass* getRenderPass() const { return _renderPass; }
		DepthBuffer* getDepthBuffer() const { return _depthBuffer; }
		ColorBuffer* getColorBuffer() const { return _colorBuffer; }

	private:
		ColorBuffer* _colorBuffer;
		DepthBuffer* _depthBuffer;
		RenderPass* _renderPass;
};

#endif// GRAPHICS_PIPELINE_H
