//--------------------------------------------------
// Robot Simulator
// uiFrameBuffer.cpp
// Date: 15/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "uiFrameBuffer.h"

UiFrameBuffer::UiFrameBuffer(ImageView* imageView, UiRenderPass* uiRenderPass)
{
	_imageView = imageView;
	_uiRenderPass = uiRenderPass;

	VkImageView attachment[1] = {_imageView->handle()};
	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = _uiRenderPass->handle();
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = attachment;
	framebufferInfo.width = _uiRenderPass->getSwapChain()->getExtent().width;
	framebufferInfo.height = _uiRenderPass->getSwapChain()->getExtent().height;
	framebufferInfo.layers = 1;

	if(vkCreateFramebuffer(_imageView->getDevice()->handle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[FrameBuffer]" << RESET << RED << " Failed to create frame buffer!" << RESET << std::endl;
		exit(1);
	}
}

UiFrameBuffer::~UiFrameBuffer()
{
	if (_framebuffer != nullptr)
	{
		vkDestroyFramebuffer(_imageView->getDevice()->handle(), _framebuffer, nullptr);
		_framebuffer = nullptr;
	}
}
