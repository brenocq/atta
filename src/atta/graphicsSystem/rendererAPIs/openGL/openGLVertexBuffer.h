//--------------------------------------------------
// Atta Graphics System
// openGLVertexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
#include <atta/graphicsSystem/vertexBuffer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>

namespace atta
{
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint8_t* data, uint32_t size, VertexBufferLayout layout, VertexBuffer::Usage usage = VertexBuffer::Usage::STATIC);
		~OpenGLVertexBuffer();

		void setData(uint8_t* data, uint32_t size, uint32_t offset) override;
		void bind() const override;

		OpenGLId getId() const { return _id; }

	private:
		static GLenum convertUsage(Usage usage);

		OpenGLId _id;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
