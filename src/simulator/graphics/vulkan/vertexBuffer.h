//--------------------------------------------------
// Robot Simulator
// vertexBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_VERTEX_BUFFER_H
#define ATTA_VK_VERTEX_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "defines.h"
#include "device.h"
#include "buffer.h"
#include "simulator/graphics/core/vertex.h"

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

#endif// ATTA_VK_VERTEX_BUFFER_H
