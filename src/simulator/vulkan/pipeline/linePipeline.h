//--------------------------------------------------
// Robot Simulator
// linePipeline.h
// Date: 2020-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LINE_PIPELINE_H
#define LINE_PIPELINE_H

#include "../vulkan.h"
#include <iostream>
#include <vector>
#include <string>

#include "pipeline.h"

class LinePipeline : public Pipeline
{
	public:
	LinePipeline(Device* device, 
			SwapChain* swapChain, 
			RenderPass* renderPass,
			std::vector<UniformBuffer*> uniformBuffers, 
			Scene* scene);
	~LinePipeline();


	private:
};

#endif// LINE_PIPELINE_H
