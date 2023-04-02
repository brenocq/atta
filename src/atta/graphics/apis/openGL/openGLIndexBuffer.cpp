//--------------------------------------------------
// Atta Graphics Module
// openGLIndexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/openGLIndexBuffer.h>

namespace atta::graphics {

OpenGLIndexBuffer::OpenGLIndexBuffer(const IndexBuffer::CreateInfo& info) : IndexBuffer(info), _id(0) {
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    if (_id > 0) {
        glDeleteBuffers(1, &_id);
        _id = 0;
    }
}

void OpenGLIndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }

} // namespace atta::graphics
