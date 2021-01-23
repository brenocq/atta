//--------------------------------------------------
// Robot Simulator
// guiRenderPass.h
// Date: 2020-07-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "guiRenderPass.h"
#include "simulator/helpers/log.h"

namespace atta
{
	GuiRenderPass::GuiRenderPass(std::shared_ptr<vk::Device> device, VkFormat colorFormat):
		_device(device), _colorFormat(colorFormat)
	{
		//----------- Color attachment ------------//
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = _colorFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		//----------- Subpass ------------//
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		//----------- SubpassDependecy ------------//
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//----------- RenderPassInfo ------------//
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if(vkCreateRenderPass(_device->handle(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
		{
			Log::error("GuiRenderPass", "Failed to create gui render pass!");
			exit(1);
		}
	}

	GuiRenderPass::~GuiRenderPass()
	{
		if(_renderPass != nullptr)
		{
			vkDestroyRenderPass(_device->handle(), _renderPass, nullptr);
			_renderPass = nullptr;
		}
	}
}
