//--------------------------------------------------
// Robot Simulator
// vertexBuffer.cpp
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "vertexBuffer.h"

VertexBuffer::VertexBuffer(std::shared_ptr<Device> device, std::vector<Vertex> vertices):
	Buffer(device, sizeof(vertices[0])*vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
}

VertexBuffer::~VertexBuffer()
{

}
