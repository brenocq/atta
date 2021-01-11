//--------------------------------------------------
// Robot Simulator
// indexBuffer.cpp
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "indexBuffer.h"

namespace atta::vk
{
	IndexBuffer::IndexBuffer(std::shared_ptr<Device> device, std::vector<unsigned int> indices):
		Buffer(device, sizeof(indices[0])*indices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	{

	}

	IndexBuffer::~IndexBuffer()
	{

	}
}
