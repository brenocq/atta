//--------------------------------------------------
// Robot Simulator
// frameBuffer.cpp
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "frameBuffer.h"
#include "swapChain.h"

FrameBuffer::FrameBuffer(ImageView* imageView, RenderPass* renderPass)
{
	_imageView = imageView;
	_renderPass = renderPass;

	VkImageView attachments[] =
	{
		_imageView->handle()
		//_renderPass->depthBuffer()->imageView()->handle()
	};

	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = _renderPass->handle();
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = attachments;
	framebufferInfo.width = _renderPass->getSwapChain()->getExtent().width;
	framebufferInfo.height = _renderPass->getSwapChain()->getExtent().height;
	framebufferInfo.layers = 1;

	if(vkCreateFramebuffer(_imageView->getDevice()->handle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[FrameBuffer]" << RESET << RED << " Failed to create frame buffer!" << RESET << std::endl;
		exit(1);
	}
}

FrameBuffer::~FrameBuffer()
{
	if (_framebuffer != nullptr)
	{
		vkDestroyFramebuffer(_imageView->getDevice()->handle(), _framebuffer, nullptr);
		_framebuffer = nullptr;
	}
}
