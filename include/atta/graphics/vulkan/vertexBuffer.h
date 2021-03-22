//--------------------------------------------------
// Robot Simulator
// vertexBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_VULKAN_VERTEX_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/buffer.h>
#include <atta/graphics/core/vertex.h>

namespace atta::vk
{
	class VertexBuffer : public Buffer
	{
		public:
			VertexBuffer(std::shared_ptr<Device> device, std::vector<Vertex> vertices);
			~VertexBuffer();

		private:
	};
}

#endif// ATTA_GRAPHICS_VULKAN_VERTEX_BUFFER_H
