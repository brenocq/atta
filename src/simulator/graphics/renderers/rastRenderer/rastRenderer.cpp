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
	}

	RastRenderer::~RastRenderer()
	{

	}

	void RastRenderer::render()
	{

	}
}
