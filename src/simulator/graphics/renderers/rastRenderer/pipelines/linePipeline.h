//--------------------------------------------------
// Robot Simulator
// linePipeline.h
// Date: 2020-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_LINE_PIPELINE_H
#define ATTA_VK_LINE_PIPELINE_H

#include <iostream>
#include <vector>
#include <string>
#include "../vulkan.h"
#include "simulator/graphics/vulkan/renderPass.h"
#include "pipeline.h"

namespace atta::vk
{
	class LinePipeline : public Pipeline
	{
		public:
			LinePipeline(std::shared_ptr<Device> device, 
					std::shared_ptr<SwapChain> swapChain, 
					RenderPass* renderPass,
					std::vector<UniformBuffer*> uniformBuffers, 
					Scene* scene);
			~LinePipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
	};
}

#endif// ATTA_VK_LINE_PIPELINE_H
