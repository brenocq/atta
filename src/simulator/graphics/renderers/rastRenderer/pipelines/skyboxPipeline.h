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
#include "simulator/graphics/vulkan/pipeline.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/graphics/vulkan/renderPass.h"

namespace atta::vk
{
	class SkyboxPipeline : public Pipeline
	{
		public:
			SkyboxPipeline(std::shared_ptr<VulkanCore> vkCore, 
					std::shared_ptr<RenderPass> renderPass,
					VkExtent2D extent, VkFormat format,
					std::vector<std::shared_ptr<ImageView>> imageViews, 
					std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, 
					std::shared_ptr<Scene> scene);
			~SkyboxPipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
			bool _environmentMapExists;
	};
}

#endif// ATTA_VK_SKY_BOX_PIPESKY_BOX_H
