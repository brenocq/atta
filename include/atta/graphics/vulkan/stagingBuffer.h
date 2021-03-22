//--------------------------------------------------
// Robot Simulator
// stagingBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_STAGING_BUFFER_H
#define ATTA_GRAPHICS_VULKAN_STAGING_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/buffer.h>
#include <atta/graphics/core/vertex.h>

namespace atta::vk
{
	class StagingBuffer : public Buffer
	{
		public:
			//template <typename T>
			//StagingBuffer(std::shared_ptr<Device> device, std::vector<T>& content);

			StagingBuffer(std::shared_ptr<Device> device, void* dataToMap, VkDeviceSize size);
			~StagingBuffer();

			void mapFromData(std::shared_ptr<Device> device, void* dataToMap, VkDeviceSize size);

		private:
	};
}

#endif// ATTA_GRAPHICS_VULKAN_STAGING_BUFFER_H
