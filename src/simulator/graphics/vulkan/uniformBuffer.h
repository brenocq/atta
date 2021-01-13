//--------------------------------------------------
// Robot Simulator
// uniformBuffer.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_UNIFORM_BUFFER_H
#define ATTA_VK_UNIFORM_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include "glm.h"
#include "defines.h"
#include "device.h"
#include "buffer.h"
#include "helpers.h"

namespace atta::vk
{
	struct UniformBufferObject
	{
		glm::mat4 modelView;
		glm::mat4 projection;
		glm::mat4 modelViewInverse;
		glm::mat4 projectionInverse;
		float aperture;
		float focusDistance;
		uint32_t totalNumberOfSamples;
		uint32_t numberOfSamples;
		uint32_t numberOfBounces;
		uint32_t randomSeed;
		uint32_t gammaCorrection; // bool
		uint32_t hasSky; // bool
	};


	class UniformBuffer : public Buffer
	{
		public:
			UniformBuffer(std::shared_ptr<Device> device, VkDeviceSize size);
			~UniformBuffer();

			void setValue(UniformBufferObject ubo);
		private:
	};
}

#endif// ATTA_VK_UNIFORM_BUFFER_H
