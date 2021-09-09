//--------------------------------------------------
// Atta Graphics System
// renderPass.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderPass.h>

namespace atta
{
	RenderPass::RenderPass(const CreateInfo& info):
		_framebuffer(info.framebuffer), _debugName(info.debugName)
	{

	}
}

