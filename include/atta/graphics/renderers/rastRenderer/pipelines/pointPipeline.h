//--------------------------------------------------
// Robot Simulator
// pointPipeline.h
// Date: 2021-04-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_POINT_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_POINT_PIPELINE_H

#include <iostream>
#include <vector>
#include <string>
#include <atta/graphics/vulkan/pipeline.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/graphics/vulkan/renderPass.h>
#include <atta/graphics/vulkan/stagingBuffer.h>

namespace atta::vk
{
	// Used to draw lines from the Drawer (helpers/drawer)
	class PointPipeline : public Pipeline
	{
		public:
			PointPipeline(std::shared_ptr<VulkanCore> vkCore, 
					std::shared_ptr<RenderPass> renderPass,
					VkExtent2D extent, VkFormat format,
					std::vector<std::shared_ptr<ImageView>> imageViews, 
					std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, 
					std::shared_ptr<Scene> scene);
			~PointPipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
	};
}

#endif// ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_PIPELINES_POINT_PIPELINE_H
