//--------------------------------------------------
// Robot Simulator
// stagingBuffer.cpp
// Date: 06/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "stagingBuffer.h"

StagingBuffer::StagingBuffer(Device* device, std::vector<Vertex> vertices):
	Buffer(device, sizeof(vertices[0])*vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	void* data;
	vkMapMemory(_device->handle(), _bufferMemory, 0, _bufferInfo.size, 0, &data);
		memcpy(data, vertices.data(), (size_t) _bufferInfo.size);
	vkUnmapMemory(_device->handle(), _bufferMemory);
}

StagingBuffer::StagingBuffer(Device* device, std::vector<uint16_t> indices):
	Buffer(device, sizeof(indices[0])*indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	void* data;
	vkMapMemory(_device->handle(), _bufferMemory, 0, _bufferInfo.size, 0, &data);
		memcpy(data, indices.data(), (size_t) _bufferInfo.size);
	vkUnmapMemory(_device->handle(), _bufferMemory);
}

StagingBuffer::StagingBuffer(Device* device, void* dataToMap, VkDeviceSize size):
	Buffer(device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	void* data;
	vkMapMemory(device, _bufferMemory, 0, _bufferInfo.size, 0, &data);
		memcpy(data, dataToMap, static_cast<size_t>(_bufferInfo.size));
	vkUnmapMemory(device, _bufferMemory);
}

StagingBuffer::~StagingBuffer()
{

}
