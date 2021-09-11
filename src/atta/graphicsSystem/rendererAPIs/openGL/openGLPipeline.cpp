//--------------------------------------------------
// Atta Graphics System
// openGLPipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLPipeline.h>

namespace atta
{
	OpenGLPipeline::OpenGLPipeline(const Pipeline::CreateInfo& info):
		Pipeline(info)
	{
		DASSERT(_renderPass, "Can not create pipeline without render pass");
		DASSERT(_shaderGroup, "Can not create pipeline without shader group");
	}

	OpenGLPipeline::~OpenGLPipeline()
	{
	}

	void OpenGLPipeline::begin()
	{
		_shaderGroup->bind();
		_renderPass->begin();
	}

	void OpenGLPipeline::end()
	{
		_renderPass->end();
		//_shaderGroup->unbind();
	}
}
