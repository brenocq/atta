//--------------------------------------------------
// Robot Simulator
// indexBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_INDEX_BUFFER_H
#define ATTA_GRAPHICS_VULKAN_INDEX_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/buffer.h>

namespace atta::vk
{
	class IndexBuffer : public Buffer
	{
		public:
			IndexBuffer(std::shared_ptr<Device> device, std::vector<unsigned int> indices);
			~IndexBuffer();

		private:
	};
}

#endif// ATTA_GRAPHICS_VULKAN_INDEX_BUFFER_H
