//--------------------------------------------------
// Robot Simulator
// pipeline.h
// Date: 2020-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "pipeline.h"

Pipeline::Pipeline(std::shared_ptr<Device> device, 
		std::vector<ImageView*> imageViews, 
		Scene* scene):
		_device(device), _imageViews(imageViews), _scene(scene)
{
}

Pipeline::~Pipeline()
{
	if(_pipeline != nullptr)
	{
		vkDestroyPipeline(_device->handle(), _pipeline, nullptr);
		_pipeline = nullptr;
	}

	if(_pipelineLayout != nullptr)
	{
		delete _pipelineLayout;
		_pipelineLayout = nullptr;
	}

	if(_descriptorSetManager != nullptr)
	{
		delete _descriptorSetManager;
		_descriptorSetManager = nullptr;
	}

	if(_vertShaderModule != nullptr)
	{
		delete _vertShaderModule;
		_vertShaderModule = nullptr;
	}

	if(_fragShaderModule != nullptr)
	{
		delete _fragShaderModule;
		_fragShaderModule = nullptr;
	}

	for(auto& frameBuffer : _frameBuffers) 
	{
		delete frameBuffer;
		frameBuffer = nullptr;
    }
}
