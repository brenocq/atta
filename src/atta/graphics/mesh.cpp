//--------------------------------------------------
// Atta Graphics Module
// mesh.cpp
// Date: 2023-04-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/mesh.h>

namespace atta::graphics {

Mesh::Mesh(CreateInfo info) : _vertexBuffer(info.vertexBuffer), _indexBuffer(info.indexBuffer) {}

} // namespace atta::graphics