#include "frameBuffer.h"
#include "depthBuffer.h"
#include "device.h"
#include "swapChain.h"
#include <array>

FrameBuffer::FrameBuffer(ImageView* imageView, RenderPass* renderPass):
	_imageView(imageView),
	_renderPass(renderPass)
{
	std::array<VkImageView, 1> attachments =
	{
		_imageView->handle()//,
		//_renderPass->depthBuffer()->imageView()->handle()
	};

	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = _renderPass->handle();
	framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebufferInfo.pAttachments = attachments.data();
	framebufferInfo.width = _renderPass->swapChain()->extent().width;
	framebufferInfo.height = _renderPass->swapChain()->extent().height;
	framebufferInfo.layers = 1;

	if(vkCreateFramebuffer(_imageView->device()->handle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[FrameBuffer] Failed to create framebuffer!" << RESET << std::endl;
		exit(1);
	}
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
	_imageView(other._imageView),
	_renderPass(other._renderPass),
	_framebuffer(other._framebuffer)
{
	other._framebuffer = nullptr;
}

FrameBuffer::~FrameBuffer()
{
	if (_framebuffer != nullptr)
		vkDestroyFramebuffer(_imageView->device()->handle(), _framebuffer, nullptr);
}
