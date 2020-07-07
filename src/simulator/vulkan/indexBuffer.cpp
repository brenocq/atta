//--------------------------------------------------
// Robot Simulator
// indexBuffer.cpp
// Date: 06/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "indexBuffer.h"

IndexBuffer::IndexBuffer(Device* device, std::vector<uint16_t> indices):
	Buffer(device, sizeof(indices[0])*indices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{

}

IndexBuffer::~IndexBuffer()
{

}
