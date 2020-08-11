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

class GraphicsPipeline : public Pipeline
{
	public:
		GraphicsPipeline(Device* device, 
				SwapChain* swapChain, 
				RenderPass* renderPass, 
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		~GraphicsPipeline();

	private:
};

#endif// GRAPHICS_PIPELINE_H
