//--------------------------------------------------
// Robot Simulator
// graphicsPipeline.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_GRAPHICS_PIPELINE_H
#define ATTA_VK_GRAPHICS_PIPELINE_H

#include <iostream>
#include <vector>
#include <string.h>

#include "simulator/graphics/vulkan/pipeline.h"
#include "simulator/graphics/vulkan/renderPass.h"
#include "simulator/graphics/vulkan/colorBuffer.h"
#include "simulator/graphics/vulkan/depthBuffer.h"

namespace atta::vk
{
	class GraphicsPipeline : public Pipeline
	{
		public:
			GraphicsPipeline(std::shared_ptr<Device> device, 
					VkExtent2D extent, VkFormat format,
					std::vector<std::shared_ptr<ImageView>> imageViews, 
					std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, 
					std::shared_ptr<Scene> scene);
			~GraphicsPipeline();

			void beginRender(VkCommandBuffer commandBuffer, int imageIndex=0);
			void endRender(VkCommandBuffer commandBuffer);
			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
			std::shared_ptr<ColorBuffer> _colorBuffer;
			std::shared_ptr<DepthBuffer> _depthBuffer;
	};
}

#endif// ATTA_VK_GRAPHICS_PIPELINE_H
