//--------------------------------------------------
// Robot Simulator
// indexBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_INDEX_BUFFER_H
#define ATTA_VK_INDEX_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "defines.h"
#include "device.h"
#include "buffer.h"

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

#endif// ATTA_VK_INDEX_BUFFER_H