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

namespace atta::vk
{
	class FrameBuffer
	{
		public:
			FrameBuffer(ImageView* imageView, RenderPass* renderPass);
			~FrameBuffer();

			VkFramebuffer handle() const { return _framebuffer; }
			ImageView* getImageView() const { return _imageView; }
			RenderPass* getRenderPass() const { return _renderPass; }

		private:
			ImageView* _imageView;
			RenderPass* _renderPass;

			VkFramebuffer _framebuffer;
	};
}

#endif// ATTA_VK_FRAME_BUFFER_H
