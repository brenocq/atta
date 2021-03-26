//--------------------------------------------------
// Atta Graphics 2D Renderer
// renderer2D.cpp
// Date: 2021-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/renderer2D/renderer2D.h>
#include <atta/helpers/log.h>
#include <atta/graphics/core/window.h>
#include <atta/graphics/core/objectInfo.h>

namespace atta
{
	Renderer2D::Renderer2D(CreateInfo info):
		Renderer({info.vkCore, info.commandPool, info.width, info.height, info.viewMat, RENDERER_TYPE_2D}), _scene(info.scene)
	{
		//---------- Create uniform buffers ----------//
		_uniformBuffer = std::make_shared<UniformBuffer2D>(_vkCore->getDevice());

		UniformBuffer2D::Data data;
		data.viewMatrix = transpose(info.viewMat);
		data.orthoMatrix = transpose(info.orthoMat); 
		_uniformBuffer->setValue(data);

		//---------- Render pass ----------//
		auto colorBuffer = std::make_shared<vk::ColorBuffer>(_vkCore->getDevice(), _image->getExtent(), _image->getFormat());
		auto depthBuffer = std::make_shared<vk::DepthBuffer>(_vkCore->getDevice(), _image->getExtent());
		_renderPass = std::make_shared<vk::RenderPass>(_vkCore->getDevice(), colorBuffer, depthBuffer);

		//---------- Frame Buffers ----------//
		_frameBuffer = std::make_shared<vk::FrameBuffer>(_vkCore->getDevice(), _imageView, _renderPass);

		//---------- Create pipelines ----------//
		_graphicsPipeline = std::make_unique<GraphicsPipeline2D>(
				_vkCore, _renderPass,
				_image->getExtent(), _image->getFormat(), 
				_imageView, _uniformBuffer, 
				_scene);
	}

	Renderer2D::~Renderer2D()
	{

	}

	void Renderer2D::render(VkCommandBuffer commandBuffer)
	{
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.5f, 0.5f, 0.5f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _renderPass->handle();
		renderPassInfo.framebuffer = _frameBuffer->handle();
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = _image->getExtent();
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		{
			// Graphics pipeline
			_graphicsPipeline->render(commandBuffer);
		}
		vkCmdEndRenderPass(commandBuffer);
	}

	void Renderer2D::updateCameraMatrix(mat4 viewMatrix)
	{
		UniformBuffer2D::Data data = _uniformBuffer->getValue();
		data.viewMatrix = transpose(viewMatrix);

		//Log::debug("R2D", "changed view: $0", inverse(viewMatrix).toString());

		mat4 ortho = transpose(data.orthoMatrix);
		mat4 view = transpose(data.viewMatrix);
		vec3 test(6.66,0,-5);
		vec3 afterView = view * test;
		vec3 afterOrtho = ortho * afterView;
		//Log::debug("Renderer2D", "view:$0ortho:$1res:$2\n$3\n$4", inverse(view).toString(), ortho.toString(), test.toString(), afterView.toString(), afterOrtho.toString());
		//Log::debug("Renderer2D", "view:$0", inverse(view).toString());

		_uniformBuffer->setValue(data);
	}
}
