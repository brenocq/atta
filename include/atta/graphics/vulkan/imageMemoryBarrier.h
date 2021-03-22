//--------------------------------------------------
// Robot Simulator
// imageMemoryBarrier.h
// Date: 2020-07-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_IMAGE_MEMORY_BARRIER_H
#define ATTA_GRAPHICS_VULKAN_IMAGE_MEMORY_BARRIER_H

#include <iostream>
#include <string.h>
#include <atta/graphics/vulkan/device.h>

namespace atta::vk
{
	class ImageMemoryBarrier final
	{
		public:

			static void insert(
				const VkCommandBuffer commandBuffer, 
				const VkImage image, 
				const VkImageSubresourceRange subresourceRange, 
				const VkAccessFlags srcAccessMask,
				const VkAccessFlags dstAccessMask, 
				const VkImageLayout oldLayout, 
				const VkImageLayout newLayout)
			{
				VkImageMemoryBarrier barrier;
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.pNext = nullptr;
				barrier.srcAccessMask = srcAccessMask;
				barrier.dstAccessMask = dstAccessMask;
				barrier.oldLayout = oldLayout;
				barrier.newLayout = newLayout;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = image;
				barrier.subresourceRange = subresourceRange;

				vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
					VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1,
					&barrier);
			}
	};
}

#endif// ATTA_GRAPHICS_VULKAN_IMAGE_MEMORY_BARRIER_H
