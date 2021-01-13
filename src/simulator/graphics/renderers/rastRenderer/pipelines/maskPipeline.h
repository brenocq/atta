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
			MaskPipeline(std::shared_ptr<Device> device, 
					std::shared_ptr<RenderPass> renderPass,
					VkExtent2D extent, VkFormat format,
					std::vector<std::shared_ptr<ImageView>> imageViews, 
					std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers, 
					std::shared_ptr<Scene> scene);
			~MaskPipeline();

			void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		private:
			// Output image info
			VkExtent2D _imageExtent;
			VkFormat _imageFormat;
	};
}

#endif// ATTA_VK_MASK_PIPELINE_H
