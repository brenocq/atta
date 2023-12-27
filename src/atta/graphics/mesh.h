//--------------------------------------------------
// Atta Graphics Module
// mesh.h
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_MESH_H
#define ATTA_GRAPHICS_MESH_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/indexBuffer.h>
#include <atta/graphics/apis/vulkan/vertexBuffer.h>

namespace atta::graphics {

class Mesh {
  public:
    struct CreateInfo {
        VertexBuffer::CreateInfo vertexBufferInfo;
        IndexBuffer::CreateInfo indexBufferInfo;
    };

    Mesh(CreateInfo info);
    virtual ~Mesh() = default;

    virtual void draw() = 0;

  protected:
    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::shared_ptr<IndexBuffer> _indexBuffer;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_MESH_H
