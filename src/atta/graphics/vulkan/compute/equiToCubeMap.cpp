//--------------------------------------------------
// Atta Graphics
// equiToCubeMap.cpp
// Date: 2021-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/compute/equiToCubeMap.h>

namespace atta::vk::compute
{
	EquiToCubeMap::EquiToCubeMap(CreateInfo info)
		: _device(info.device), _commandPool(info.commandPool), _input(info.input)
	{
		//---------- Create output image ----------//
		unsigned size = _input->getImage()->getExtent().height;
		_output = std::make_shared<vk::Texture>(_device, _commandPool, 
				(VkExtent2D){size, size}, 
				_input->getImage()->getFormat(), 
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				true/*isCubemap*/);
	}

	EquiToCubeMap::~EquiToCubeMap()
	{

	}

	void EquiToCubeMap::compute()
	{

	}
}
