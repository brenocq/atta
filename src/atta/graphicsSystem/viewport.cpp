//--------------------------------------------------
// Atta Graphics System
// viewport.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/viewport.h>

namespace atta
{
	Viewport::Viewport(CreateInfo info):
		_sid(info.sid), _renderer(info.renderer)
	{

	}

	Viewport::~Viewport()
	{

	}

	void Viewport::render()
	{
		if(_renderer != nullptr)
			_renderer->render();
	}

	void Viewport::resize(uint32_t width, uint32_t height)
	{
		if(_renderer != nullptr)
			_renderer->resize(width, height);
	}
}
