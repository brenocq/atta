#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "imageView.h"
#include "renderPass.h"

class FrameBuffer
{
	public:

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator = (const FrameBuffer&) = delete;
		FrameBuffer& operator = (FrameBuffer&&) = delete;

		FrameBuffer(ImageView* imageView, RenderPass* renderPass);
		FrameBuffer(FrameBuffer&& other) noexcept;
		~FrameBuffer();

		VkFramebuffer handle() const { return _framebuffer; }
		ImageView* imageView() const { return _imageView; }
		RenderPass* renderPass() const { return _renderPass; }

	private:
		ImageView* _imageView;
		RenderPass* _renderPass;

		VkFramebuffer _framebuffer;
};

#endif// FRAME_BUFFER_H
