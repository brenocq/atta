//--------------------------------------------------
// Robot Simulator
// frameBuffer.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "frameBuffer.h"
#include "swapChain.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	FrameBuffer::FrameBuffer(ImageView* imageView, RenderPass* renderPass)
	{
		_imageView = imageView;
		_renderPass = renderPass;

		std::array<VkImageView, 3> attachments = {
			_renderPass->getColorBuffer()->getImageView()->handle(),
			_renderPass->getDepthBuffer()->getImageView()->handle(),
			_imageView->handle(),
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass->handle();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _renderPass->getColorBuffer()->getExtent().width;
		framebufferInfo.height = _renderPass->getColorBuffer()->getExtent().height;
		framebufferInfo.layers = 1;

		if(vkCreateFramebuffer(_imageView->getDevice()->handle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)
		{
			Log::error("FrameBuffer", "Failed to create frame buffer!");
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
}
