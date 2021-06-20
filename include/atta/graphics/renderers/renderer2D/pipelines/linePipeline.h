//--------------------------------------------------
// Robot Simulator
// linePipeline.h
// Date: 2020-08-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_RENDERER_2D_PIPELINES_LINE_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_RENDERER_2D_PIPELINES_LINE_PIPELINE_H

#include <iostream>
#include <vector>
#include <string>
#include <atta/graphics/vulkan/pipeline.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/graphics/vulkan/renderPass.h>
#include <atta/graphics/vulkan/stagingBuffer.h>
#include <atta/graphics/renderers/renderer2D/uniformBuffer2D.h>

namespace atta
{
	// Used to draw lines from the Drawer (helpers/drawer)
	class LinePipeline : public vk::Pipeline
	{
		public:
			LinePipeline(std::shared_ptr<vk::VulkanCore> vkCore, 
					std::shared_ptr<vk::RenderPass> renderPass,
					VkExtent2D extent, VkFormat format,
					std::vector<std::shared_ptr<vk::ImageView>> imageViews, 
					std::vector<std::shared_ptr<UniformBuffer2D>> uniformBuffers, 
					std::shared_ptr<Scene> scene);
			~LinePipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
	};
}

#endif// ATTA_GRAPHICS_RENDERERS_RENDERER_2D_PIPELINES_LINE_PIPELINE_H
