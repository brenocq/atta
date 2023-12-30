//--------------------------------------------------
// Atta Graphics Module
// indexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/indexBuffer.h>

namespace atta::graphics::gl {

IndexBuffer::IndexBuffer(const IndexBuffer::CreateInfo& info) : gfx::IndexBuffer(info), _id(0) {
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    if (_id > 0) {
        glDeleteBuffers(1, &_id);
        _id = 0;
    }
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }

} // namespace atta::graphics::gl
