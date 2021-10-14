//--------------------------------------------------
// Atta Graphics System
// openGLMesh.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_MESH_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_MESH_H
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLVertexBuffer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLIndexBuffer.h>
#include <atta/core/stringId.h>

namespace atta
{
    class OpenGLMesh final
    {
    public:
        OpenGLMesh(StringId sid);
        ~OpenGLMesh();

        void draw();

    private:
        std::shared_ptr<OpenGLVertexBuffer> _vertexBuffer;
        std::shared_ptr<OpenGLIndexBuffer> _indexBuffer;
        StringId _sid;

        OpenGLId _id;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_MESH_H
