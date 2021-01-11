//--------------------------------------------------
// Robot Simulator
// stagingBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_STAGING_BUFFER_H
#define ATTA_VK_STAGING_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "defines.h"
#include "device.h"
#include "buffer.h"
#include "simulator/graphics/core/vertex.h"

namespace atta::vk
{
	class StagingBuffer : public Buffer
	{
		public:
		template <class T>
		StagingBuffer(std::shared_ptr<Device> device, std::vector<T>& content);

		StagingBuffer(std::shared_ptr<Device> device, void* dataToMap, VkDeviceSize size);
		~StagingBuffer();

		private:
	};
}

#endif// ATTA_VK_STAGING_BUFFER_H
