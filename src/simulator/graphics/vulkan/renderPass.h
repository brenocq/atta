//--------------------------------------------------
// Robot Simulator
// renderPass.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_RENDER_PASS_H
#define ATTA_VK_RENDER_PASS_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "swapChain.h"
#include "depthBuffer.h"
#include "colorBuffer.h"

namespace atta::vk
{
	class RenderPass
	{
		public:
			RenderPass(std::shared_ptr<Device> device, DepthBuffer* depthBuffer, ColorBuffer* colorBuffer);
			~RenderPass();

			VkRenderPass handle() const { return _renderPass; }
			DepthBuffer* getDepthBuffer() const { return _depthBuffer; }
			ColorBuffer* getColorBuffer() const { return _colorBuffer; }

		private:
			VkRenderPass _renderPass;
			std::shared_ptr<Device> _device;
			DepthBuffer* _depthBuffer;
			ColorBuffer* _colorBuffer;
	};
}

#endif// ATTA_VK_RENDER_PASS_H
