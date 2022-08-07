//--------------------------------------------------
// Atta Graphics Module
// openGLVertexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
#include <atta/graphics/vertexBuffer.h>
#include <atta/graphics/rendererAPIs/openGL/base.h>

namespace atta
{
    class OpenGLVertexBuffer final : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const VertexBuffer::CreateInfo& info);
        ~OpenGLVertexBuffer();

        void bind() const override;

        OpenGLId getId() const { return _id; }

    private:
        static GLenum convertUsage(Usage usage);
        static GLenum convertBaseType(VertexBufferElement::Type type);

        OpenGLId _id;
    };
}

#endif// ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_VERTEX_BUFFER_H
