//--------------------------------------------------
// Robot Simulator
// outlinePipeline.h
// Date: 2020-11-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_OUTLINE_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_OUTLINE_PIPELINE_H

#include <iostream>
#include <vector>
#include <string.h>
#include <atta/graphics/vulkan/pipeline.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/graphics/vulkan/renderPass.h>
#include <atta/graphics/vulkan/colorBuffer.h>
#include <atta/graphics/vulkan/depthBuffer.h>

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

#endif// ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_OUTLINE_PIPELINE_H
