//--------------------------------------------------
// Robot Simulator
// guiFrameBuffer.h
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_FRAME_BUFFER_H
#define GUI_FRAME_BUFFER_H

#include "../imageView.h"
#include "guiRenderPass.h"

class GuiFrameBuffer
{
	public:
		GuiFrameBuffer(ImageView* imageView, GuiRenderPass* guiRenderPass, VkExtent2D imageExtent);
		~GuiFrameBuffer();

		VkFramebuffer handle() const { return _framebuffer; }
		ImageView* getImageView() const { return _imageView; }
		GuiRenderPass* getGuiRenderPass() const { return _guiRenderPass; }

	private:
		ImageView* _imageView;
		GuiRenderPass* _guiRenderPass;

		VkFramebuffer _framebuffer;
		VkExtent2D _imageExtent;
};

#endif// GUI_FRAME_BUFFER_H
