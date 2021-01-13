//--------------------------------------------------
// Atta Graphics
// rastRenderer.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "rastRenderer.h"

namespace atta
{
	RastRenderer::RastRenderer(CreateInfo info):
		Renderer({info.vkCore, info.width, info.height}), _scene(info.scene)
	{
		// Create pipelines
		_graphicsPipeline = std::make_unique<vk::GraphicsPipeline>(
				_device, _image->getExtent(), _image->getFormat(), {_imageView}, 
				_scene);
		_linePipeline = std::make_unique<vk::LinePipeline>(
				_device, _image->getExtent(), _image->getFormat(), {_imageView}, 
				_graphicsPipeline->getRenderPass(), _scene);
		_maskPipeline = std::make_unique<vk::MaskPipeline>(
				_device, _image->getExtent(), _image->getFormat(), {_imageView}, 
				_graphicsPipeline->getRenderPass(), _scene);
		_outlinePipeline = std::make_unique<vk::OutlinePipeline>(
				_device, _image->getExtent(), _image->getFormat(), {_imageView}, 
				_graphicsPipeline->getRenderPass(), _scene);
		_skyboxPipeline = std::make_unique<vk::SkyboxPipeline>(
				_device, _image->getExtent(), _image->getFormat(), {_imageView}, 
				_graphicsPipeline->getRenderPass(), _scene);
	}

	RastRenderer::~RastRenderer()
	{

	}

	void RastRenderer::render()
	{

	}
}
