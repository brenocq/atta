//--------------------------------------------------
// Robot Simulator
// skyboxPipeline.h
// Date: 2020-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_SKY_BOX_PIPESKY_BOX_H
#define ATTA_VK_SKY_BOX_PIPESKY_BOX_H

#include <iostream>
#include <vector>
#include <string>
#include "../vulkan.h"
#include "simulator/graphics/vulkan/renderPass.h"
#include "pipeline.h"

namespace atta::vk
{
	class SkyboxPipeline : public Pipeline
	{
		public:
			SkyboxPipeline(std::shared_ptr<Device> device, 
					std::shared_ptr<SwapChain> swapChain, 
					std::shared_ptr<RenderPass> renderPass,
					std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, 
					std::shared_ptr<Scene> scene);
			~SkyboxPipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
	};
}

#endif// ATTA_VK_SKY_BOX_PIPESKY_BOX_H
