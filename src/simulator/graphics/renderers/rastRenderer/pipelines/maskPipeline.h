//--------------------------------------------------
// Robot Simulator
// maskPipeline.h
// Date: 2020-11-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_MASK_PIPELINE_H
#define ATTA_VK_MASK_PIPELINE_H

#include <iostream>
#include <vector>
#include <string.h>

#include "pipeline.h"
#include "simulator/graphics/vulkan/renderPass.h"

namespace atta::vk
{
	class MaskPipeline : public Pipeline
	{
		public:
			// Graphics Pipeline with swapchain
			MaskPipeline(std::shared_ptr<Device> device, 
					std::shared_ptr<SwapChain> swapChain, 
					RenderPass* renderPass,
					std::vector<UniformBuffer*> uniformBuffers, 
					Scene* scene);
			// Offline Graphics Pipeline
			MaskPipeline(std::shared_ptr<Device> device, 
					RenderPass* renderPass,
					VkExtent2D extent, VkFormat format,
					ImageView* imageView,
					UniformBuffer* uniformBuffer, 
					Scene* scene);
			// Base constructor
			MaskPipeline(std::shared_ptr<Device> device, 
					RenderPass* renderPass,
					VkExtent2D extent, VkFormat format,
					std::vector<ImageView*> imageViews, 
					std::vector<UniformBuffer*> uniformBuffers, 
					Scene* scene);
			~MaskPipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
			// Output image info
			VkExtent2D _imageExtent;
			VkFormat _imageFormat;
	};
}

#endif// ATTA_VK_MASK_PIPELINE_H
