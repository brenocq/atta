//--------------------------------------------------
// Robot Simulator
// guiFrameBuffer.cpp
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "guiFrameBuffer.h"
#include "simulator/helpers/log.h"

GuiFrameBuffer::GuiFrameBuffer(ImageView* imageView, GuiRenderPass* guiRenderPass, VkExtent2D imageExtent):
	_imageExtent(imageExtent)
{
	_imageView = imageView;
	_guiRenderPass = guiRenderPass;

	VkImageView attachment[1] = {_imageView->handle()};
	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = _guiRenderPass->handle();
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = attachment;
	framebufferInfo.width = _imageExtent.width;
	framebufferInfo.height = _imageExtent.height;
	framebufferInfo.layers = 1;

	if(vkCreateFramebuffer(_imageView->getDevice()->handle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)
	{
		Log::error("GuiFrameBuffer", "Failed to create gui frame buffer!");
		exit(1);
	}
}

GuiFrameBuffer::~GuiFrameBuffer()
{
	if (_framebuffer != nullptr)
	{
		vkDestroyFramebuffer(_imageView->getDevice()->handle(), _framebuffer, nullptr);
		_framebuffer = nullptr;
	}
}
