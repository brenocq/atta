//--------------------------------------------------
// Atta Graphics System
// openGLPipeline.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_PIPELINE_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_PIPELINE_H
#include <atta/graphicsSystem/pipeline.h>

namespace atta
{
	class OpenGLPipeline final : public Pipeline
	{
	public:
		OpenGLPipeline(const Pipeline::CreateInfo& info);
		~OpenGLPipeline();
	
		void bind() override;

	private:
		static GLenum convertBaseType(VertexBufferElement::Type type);

		OpenGLId _vertexArrayId;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_PIPELINE_H
