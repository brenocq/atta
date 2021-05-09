//--------------------------------------------------
// Robot Simulator
// guiFrameBuffer.cpp
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/guiFrameBuffer.h>
#include <atta/helpers/log.h>

namespace atta
{
	GuiFrameBuffer::GuiFrameBuffer(
			std::shared_ptr<vk::Device> device, 
			std::shared_ptr<vk::ImageView> imageView, 
			std::shared_ptr<vk::ImageView> depthImageView, 
			std::shared_ptr<vk::ImageView> colorImageView, 
			std::shared_ptr<GuiRenderPass> guiRenderPass, 
			VkExtent2D imageExtent):
		_device(device),
		_imageView(imageView),
		_depthImageView(depthImageView),
		_colorImageView(colorImageView),
		_guiRenderPass(guiRenderPass),
		_imageExtent(imageExtent)
	{
		// For now the GUI will not have multisampling and images with VK_1_SAMPLE_BIT will be copy to the swapchain image
		bool useMultisampling = false;

		std::vector<VkImageView> attachments;
		if(useMultisampling)
		{
			// Render to color and depth images, then save multisampling result to swapchain image
			attachments.push_back(_colorImageView->handle());
			attachments.push_back(_depthImageView->handle());
			attachments.push_back(_imageView->handle());
		}
		else
		{
			// Render directly to the swapchain image
			attachments.push_back(_imageView->handle());
			attachments.push_back(_depthImageView->handle());
		}
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _guiRenderPass->handle();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _imageExtent.width;
		framebufferInfo.height = _imageExtent.height;
		framebufferInfo.layers = 1;

		if(vkCreateFramebuffer(_device->handle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS)
		{
			Log::error("GuiFrameBuffer", "Failed to create gui frame buffer!");
			exit(1);
		}
	}

	GuiFrameBuffer::~GuiFrameBuffer()
	{
		if (_framebuffer != nullptr)
		{
			vkDestroyFramebuffer(_device->handle(), _framebuffer, nullptr);
			_framebuffer = nullptr;
		}
	}
}
