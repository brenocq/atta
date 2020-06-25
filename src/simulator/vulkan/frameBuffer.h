//--------------------------------------------------
// Robot Simulator
// frameBuffer.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "imageView.h"
#include "renderPass.h"

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

#endif// FRAME_BUFFER_H
