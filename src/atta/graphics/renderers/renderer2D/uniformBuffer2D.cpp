//--------------------------------------------------
// Atta Graphics 2D Renderer
// uniformBuffer2D.cpp
// Date: 2021-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/renderer2D/uniformBuffer2D.h>
#include <atta/graphics/core/window.h>
#include <cstring>

namespace atta
{
	UniformBuffer2D::UniformBuffer2D(std::shared_ptr<vk::Device> device):
		Buffer(device, sizeof(Data), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	{
		Data data;
		data.viewMatrix = mat4(1);
		data.orthoMatrix = orthographic(10.0f, Window::ratio);
		setValue(data);
	}

	UniformBuffer2D::~UniformBuffer2D()
	{
	}

	void UniformBuffer2D::setValue(Data data)
	{
		_data = data;
		const auto mem = mapMemory(0, sizeof(Data));
		std::memcpy(mem, &data, sizeof(Data));
		unmapMemory();
	}
}
