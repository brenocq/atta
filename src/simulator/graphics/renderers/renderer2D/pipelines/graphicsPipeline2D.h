//--------------------------------------------------
// Atta Graphics 2D Renderer
// graphicsPipeline2D.h
// Date: 2021-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_PIPELINE_2D_H
#define ATTA_GRAPHICS_PIPELINE_2D_H

#include "simulator/graphics/vulkan/pipeline.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/graphics/vulkan/renderPass.h"
#include "../uniformBuffer2D.h"

namespace atta
{
	class GraphicsPipeline2D : public vk::Pipeline
	{
		public:
			GraphicsPipeline2D(std::shared_ptr<vk::VulkanCore> vkCore, 
					std::shared_ptr<vk::RenderPass> renderPass,
					VkExtent2D extent, VkFormat format,
					std::shared_ptr<vk::ImageView> imageView,
					std::shared_ptr<UniformBuffer2D> uniformBuffer,
					std::shared_ptr<Scene> scene);
			~GraphicsPipeline2D();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
	};
}

#endif// ATTA_GRAPHICS_PIPELINE_2D_H
