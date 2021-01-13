//--------------------------------------------------
// Robot Simulator
// frameBuffer.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VK_FRAME_BUFFER_H
#define ATTA_VK_FRAME_BUFFER_H

#include "imageView.h"
#include "renderPass.h"
#include "device.h"

namespace atta::vk
{
	class FrameBuffer
	{
		public:
			FrameBuffer(std::shared_ptr<Device> device, std::shared_ptr<ImageView> imageView, std::shared_ptr<RenderPass> renderPass);
			~FrameBuffer();

			VkFramebuffer handle() const { return _framebuffer; }
			std::shared_ptr<ImageView> getImageView() const { return _imageView; }
			std::shared_ptr<RenderPass> getRenderPass() const { return _renderPass; }

		private:
			VkFramebuffer _framebuffer;
			std::shared_ptr<Device> _device;
			std::shared_ptr<ImageView> _imageView;
			std::shared_ptr<RenderPass> _renderPass;
	};
}

#endif// ATTA_VK_FRAME_BUFFER_H
