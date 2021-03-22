//--------------------------------------------------
// Robot Simulator
// pipeline.h
// Date: 2020-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/pipeline.h>

namespace atta::vk
{
	Pipeline::Pipeline(
			std::shared_ptr<VulkanCore> vkCore, 
			std::vector<std::shared_ptr<ImageView>> imageViews, 
			std::shared_ptr<Scene> scene):
			_vkCore(vkCore), _device(vkCore->getDevice()), _imageViews(imageViews), _scene(scene)
	{
	}

	Pipeline::~Pipeline()
	{
		if(_pipeline != nullptr)
		{
			vkDestroyPipeline(_device->handle(), _pipeline, nullptr);
			_pipeline = nullptr;
		}
	}
}
