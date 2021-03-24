//--------------------------------------------------
// Robot Simulator
// skyboxPipeline.h
// Date: 2020-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_SKY_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_SKY_PIPELINE_H

#include <iostream>
#include <vector>
#include <string>
#include <atta/graphics/vulkan/pipeline.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/graphics/vulkan/renderPass.h>

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

#endif// ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_SKY_PIPELINE_H
