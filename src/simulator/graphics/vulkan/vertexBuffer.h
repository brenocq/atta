//--------------------------------------------------
// Robot Simulator
// vertexBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "defines.h"
#include "device.h"
#include "buffer.h"
#include "simulator/graphics/core/vertex.h"

class VertexBuffer : public Buffer
{
	public:
		VertexBuffer(std::shared_ptr<Device> device, std::vector<Vertex> vertices);
		~VertexBuffer();

	private:
};

#endif// VERTEX_BUFFER_H
