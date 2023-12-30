//--------------------------------------------------
// Atta Graphics Module
// mesh.h
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_MESH_H
#define ATTA_GRAPHICS_MESH_H

#include <atta/graphics/indexBuffer.h>
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics {

class Mesh {
  public:
    /**
     * @brief Mesh create info
     *
     * @note Index buffer can be empty if indices should not be used while drawing
     */
    struct CreateInfo {
        VertexBuffer::CreateInfo vertexBufferInfo;
        IndexBuffer::CreateInfo indexBufferInfo;
    };

    Mesh(CreateInfo info);
    virtual ~Mesh() = default;

    virtual void draw() = 0;

  protected:
    std::shared_ptr<VertexBuffer> _vertexBuffer; ///< Vertex buffer
    std::shared_ptr<IndexBuffer> _indexBuffer;   ///< Index buffer (may be nullptr)
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_MESH_H
