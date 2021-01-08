//--------------------------------------------------
// Robot Simulator
// skyboxPipeline.h
// Date: 2020-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SKY_BOX_PIPESKY_BOX_H
#define SKY_BOX_PIPESKY_BOX_H

#include <iostream>
#include <vector>
#include <string>
#include "../vulkan.h"
#include "simulator/graphics/vulkan/renderPass.h"
#include "pipeline.h"

class SkyboxPipeline : public Pipeline
{
	public:
		SkyboxPipeline(std::shared_ptr<Device> device, 
				std::shared_ptr<SwapChain> swapChain, 
				RenderPass* renderPass,
				std::vector<UniformBuffer*> uniformBuffers, 
				Scene* scene);
		~SkyboxPipeline();

		void render(VkCommandBuffer commandBuffer, int imageIndex=0);

	private:
};

#endif// SKY_BOX_PIPESKY_BOX_H
