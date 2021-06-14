//--------------------------------------------------
// Atta Graphics
// rastRenderer.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/rastRenderer/rastRenderer.h>
#include <atta/helpers/log.h>
//#include "glm.h"

namespace atta
{
	RastRenderer::RastRenderer(CreateInfo info):
		Renderer({info.vkCore, info.commandPool, info.width, info.height, info.viewMat, RENDERER_TYPE_RASTERIZATION}), _scene(info.scene),
		_fov(info.fov)
	{
		_linePipelineSupport = _vkCore->getDevice()->getPhysicalDevice()->getSupport().fillModeNonSolidFeature;

		//---------- Create uniform buffers ----------//
		_uniformBuffer = std::make_shared<vk::UniformBuffer>(_vkCore->getDevice());

		vk::UniformBufferObject ubo;
		ubo.viewMat = atta::transpose(info.viewMat);
		ubo.projMat = atta::transpose(atta::perspective(atta::radians(info.fov), info.width/info.height, 0.01f, 1000.0f));
		ubo.projMat.data[5] *= -1;
		ubo.viewMatInverse = atta::inverse(ubo.viewMat);
		ubo.projMatInverse = atta::inverse(ubo.projMat);
		ubo.nLights = _scene->getLights().size();
		_uniformBuffer->setValue(ubo);

		//---------- Create pipelines ----------//
		createRenderPass();
		createFrameBuffers();
		createPipelines();
	}

	RastRenderer::~RastRenderer()
	{

	}

	void RastRenderer::createRenderPass()
	{
		auto colorBuffer = std::make_shared<vk::ColorBuffer>(_vkCore->getDevice(), _image->getExtent(), _image->getFormat());
		auto depthBuffer = std::make_shared<vk::DepthBuffer>(_vkCore->getDevice(), _image->getExtent());
		_renderPass = std::make_shared<vk::RenderPass>(_vkCore->getDevice(), colorBuffer, depthBuffer);
	}

	void RastRenderer::createFrameBuffers()
	{
		_frameBuffer = std::make_shared<vk::FrameBuffer>(_vkCore->getDevice(), _imageView, _renderPass);
	}

	void RastRenderer::createPipelines()
	{
		_graphicsPipeline = std::make_unique<vk::GraphicsPipeline>(
				_vkCore, _renderPass,
				_image->getExtent(), _image->getFormat(), 
				std::vector<std::shared_ptr<vk::ImageView>>({_imageView}), 
				std::vector<std::shared_ptr<vk::UniformBuffer>>({_uniformBuffer}), 
				_scene);
		if(_linePipelineSupport)
			_linePipeline = std::make_unique<vk::LinePipeline>(
					_vkCore, _renderPass,
					_image->getExtent(), _image->getFormat(), 
					std::vector<std::shared_ptr<vk::ImageView>>({_imageView}), 
					std::vector<std::shared_ptr<vk::UniformBuffer>>({_uniformBuffer}), 
					_scene);
		else
			_linePipeline = nullptr;

		_pointPipeline = std::make_unique<vk::PointPipeline>(
				_vkCore, _renderPass,
				_image->getExtent(), _image->getFormat(), 
				std::vector<std::shared_ptr<vk::ImageView>>({_imageView}), 
				std::vector<std::shared_ptr<vk::UniformBuffer>>({_uniformBuffer}), 
				_scene);

		//_maskPipeline = std::make_unique<vk::MaskPipeline>(
		//		_vkCore, _renderPass,
		//		_image->getExtent(), _image->getFormat(), 
		//		std::vector<std::shared_ptr<vk::ImageView>>({_imageView}), 
		//		std::vector<std::shared_ptr<vk::UniformBuffer>>({_uniformBuffer}), 
		//		_scene);
		//_outlinePipeline = std::make_unique<vk::OutlinePipeline>(
		//		_vkCore, _renderPass,
		//		_image->getExtent(), _image->getFormat(), 
		//		std::vector<std::shared_ptr<vk::ImageView>>({_imageView}), 
		//		std::vector<std::shared_ptr<vk::UniformBuffer>>({_uniformBuffer}), 
		//		_scene);
		//_skyboxPipeline = std::make_unique<vk::SkyboxPipeline>(
		//		_vkCore, _renderPass,
		//		_image->getExtent(), _image->getFormat(), 
		//		std::vector<std::shared_ptr<vk::ImageView>>({_imageView}), 
		//		std::vector<std::shared_ptr<vk::UniformBuffer>>({_uniformBuffer}), 
		//		_scene);
	}

	void RastRenderer::render(VkCommandBuffer commandBuffer)
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
			// Skybox pipeline
			//_skyboxPipeline->render(commandBuffer);

			// Graphics pipeline
			_graphicsPipeline->render(commandBuffer);

			// Point pipeline
			_pointPipeline->render(commandBuffer);

			// Line pipeline
			if(_linePipelineSupport)
				_linePipeline->render(commandBuffer);

			// Mask pipeline
			//_maskPipeline->render(commandBuffer);

			// Outline pipeline
			//_outlinePipeline->render(commandBuffer);
		}
		vkCmdEndRenderPass(commandBuffer);
	}

	void RastRenderer::updateCameraMatrix(mat4 viewMatrix)
	{
		vk::UniformBufferObject ubo = _uniformBuffer->getValue();
		ubo.viewMat = atta::transpose(viewMatrix);
		ubo.viewMatInverse = atta::inverse(ubo.viewMat);

		//Log::debug("RastRenderer", "ViewInverse: $0", ubo.viewMatInverse.toString());
		//Log::debug("RastRenderer", "Proj: $0", ubo.projMat.toString());
		//Log::debug("RastRenderer", "ProjInverse: $0", ubo.projMatInverse.toString());
		_uniformBuffer->setValue(ubo);
	}

	void RastRenderer::resize(unsigned width, unsigned height)
	{
		_extent.width = width;
		_extent.height = height;

		_linePipeline.reset();
		_pointPipeline.reset();
		_graphicsPipeline.reset();
		_frameBuffer.reset();
		_renderPass.reset();
		_imageView.reset();
		_image.reset();

		createOutputImage();
		createRenderPass();
		createFrameBuffers();
		createPipelines();

		// Update uniform buffer projection matrix
		vk::UniformBufferObject ubo  = _uniformBuffer->getValue();
		ubo.projMat = atta::transpose(atta::perspective(atta::radians(_fov), width/(float)height, 0.01f, 1000.0f));
		ubo.projMat.data[5] *= -1;
		ubo.projMatInverse = atta::inverse(ubo.projMat);
		_uniformBuffer->setValue(ubo);
	}
}
