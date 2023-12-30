//--------------------------------------------------
// Atta Graphics Module
// mesh.cpp
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/mesh.h>

namespace atta::graphics {

Mesh::Mesh(CreateInfo info) {}

std::shared_ptr<VertexBuffer> Mesh::getVertexBuffer() const { return _vertexBuffer; }
std::shared_ptr<IndexBuffer> Mesh::getIndexBuffer() const { return _indexBuffer; }

} // namespace atta::graphics
