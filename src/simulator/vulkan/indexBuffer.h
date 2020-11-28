//--------------------------------------------------
// Robot Simulator
// indexBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "defines.h"
#include "device.h"
#include "buffer.h"

class IndexBuffer : public Buffer
{
	public:
	IndexBuffer(Device* device, std::vector<unsigned int> indices);
	~IndexBuffer();

	private:
};

#endif// INDEX_BUFFER_H
