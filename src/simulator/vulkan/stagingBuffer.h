//--------------------------------------------------
// Robot Simulator
// stagingBuffer.h
// Date: 06/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef STAGING_BUFFER_H
#define STAGING_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "defines.h"
#include "device.h"
#include "buffer.h"
#include "vertex.h"

class StagingBuffer : public Buffer
{
	public:
	template <class T>
	StagingBuffer(Device* device, std::vector<T>& content);

	StagingBuffer(Device* device, void* dataToMap, VkDeviceSize size);
	~StagingBuffer();

	private:
};

#endif// STAGING_BUFFER_H
