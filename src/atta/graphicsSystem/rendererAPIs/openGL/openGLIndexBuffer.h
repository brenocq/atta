//--------------------------------------------------
// Atta Graphics System
// openGLIndexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_INDEX_BUFFER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_INDEX_BUFFER_H
#include <atta/graphicsSystem/indexBuffer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>

namespace atta
{
	class OpenGLIndexBuffer final : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const IndexBuffer::CreateInfo& info, OpenGLId vao);
		~OpenGLIndexBuffer();

		void bind() const override;

		OpenGLId getId() const { return _id; }

	private:
		OpenGLId _id;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_INDEX_BUFFER_H