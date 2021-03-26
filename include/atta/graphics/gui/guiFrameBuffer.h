//--------------------------------------------------
// Robot Simulator
// guiFrameBuffer.h
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_FRAME_BUFFER_H
#define GUI_FRAME_BUFFER_H

#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/imageView.h>
#include <atta/graphics/gui/guiRenderPass.h>

namespace atta
{
	class GuiFrameBuffer
	{
		public:
			GuiFrameBuffer(std::shared_ptr<vk::Device> device, std::shared_ptr<vk::ImageView> imageView, std::shared_ptr<GuiRenderPass> guiRenderPass, VkExtent2D imageExtent);
			~GuiFrameBuffer();

			VkFramebuffer handle() const { return _framebuffer; }
			std::shared_ptr<vk::ImageView> getImageView() const { return _imageView; }
			std::shared_ptr<GuiRenderPass> getGuiRenderPass() const { return _guiRenderPass; }

		private:
			std::shared_ptr<vk::Device> _device;
			std::shared_ptr<vk::ImageView> _imageView;
			std::shared_ptr<GuiRenderPass> _guiRenderPass;

			VkFramebuffer _framebuffer;
			VkExtent2D _imageExtent;
	};
}
#endif// GUI_FRAME_BUFFER_H
