//--------------------------------------------------
// Robot Simulator
// outlinePipeline.h
// Date: 2020-11-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_OUTLINE_PIPELINE_H
#define ATTA_VK_OUTLINE_PIPELINE_H

#include <iostream>
#include <vector>
#include <string.h>
#include "simulator/graphics/vulkan/pipeline.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/graphics/vulkan/renderPass.h"
#include "simulator/graphics/vulkan/colorBuffer.h"
#include "simulator/graphics/vulkan/depthBuffer.h"

namespace atta::vk
{
	class OutlinePipeline : public Pipeline
	{
		public:
			OutlinePipeline(std::shared_ptr<VulkanCore> vkCore, 
					std::shared_ptr<RenderPass> renderPass,
					VkExtent2D extent, VkFormat format,
					std::vector<std::shared_ptr<ImageView>> imageViews, 
					std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, 
					std::shared_ptr<Scene> scene);
			~OutlinePipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
	};
}

#endif// ATTA_VK_OUTLINE_PIPELINE_H
