//--------------------------------------------------
// Robot Simulator
// uiFrameBuffer.h
// Date: 15/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef UI_FRAME_BUFFER_H
#define UI_FRAME_BUFFER_H

#include "../imageView.h"
#include "uiRenderPass.h"

class UiFrameBuffer
{
	public:
		UiFrameBuffer(ImageView* imageView, UiRenderPass* uiRenderPass);
		~UiFrameBuffer();

		VkFramebuffer handle() const { return _framebuffer; }
		ImageView* getImageView() const { return _imageView; }
		UiRenderPass* getUiRenderPass() const { return _uiRenderPass; }

	private:
		ImageView* _imageView;
		UiRenderPass* _uiRenderPass;

		VkFramebuffer _framebuffer;
};

#endif// UI_FRAME_BUFFER_H
