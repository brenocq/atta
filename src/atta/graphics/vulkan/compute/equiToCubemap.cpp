//--------------------------------------------------
// Atta Graphics
// equiToCubemap.cpp
// Date: 2021-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/compute/equiToCubemap.h>

namespace atta::vk::compute
{
	EquiToCubemap::EquiToCubemap(CreateInfo info)
		: _device(info.device), _commandPool(info.commandPool), _input(info.input)
	{
	}

	void EquiToCubemap::compute()
	{
	}
}
