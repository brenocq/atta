//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/vertexBuffer.h>

namespace atta::graphics::gl {

VertexBuffer::VertexBuffer(const VertexBuffer::CreateInfo& info) : gfx::VertexBuffer(info), _id(0) {
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);

    uint32_t i = 0;
    for (const auto& element : _layout.getElements()) {
        GLenum openGLType = convertBaseType(element.type);

        // Define attribute format
        if (openGLType == GL_INT) {
            glVertexAttribIPointer(i, element.getComponentCount(), openGLType, _layout.getStride(), reinterpret_cast<void*>(element.offset));
        } else {
            glVertexAttribPointer(i, element.getComponentCount(), openGLType, element.normalized ? GL_TRUE : GL_FALSE, _layout.getStride(),
                                  reinterpret_cast<void*>(element.offset));
        }

        // Enable attribute
        glEnableVertexAttribArray(i);
        i++;
    }
}

VertexBuffer::~VertexBuffer() {
    if (_id > 0) {
        glDeleteBuffers(1, &_id);
        _id = 0;
    }
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, _id); }

GLenum VertexBuffer::convertUsage(Usage usage) {
    switch (usage) {
        case Usage::STATIC:
            return GL_STATIC_DRAW;
        case Usage::DYNAMIC:
            return GL_DYNAMIC_DRAW;
        default:
            break;
    }
    ASSERT(false, "Unknown vertex buffer usage");
}

GLenum VertexBuffer::convertBaseType(VertexBufferElement::Type type) {
    switch (type) {
        case VertexBufferElement::Type::BOOL:
            return GL_BOOL;
        case VertexBufferElement::Type::INT:
        case VertexBufferElement::Type::UINT:
        case VertexBufferElement::Type::IVEC2:
        case VertexBufferElement::Type::IVEC3:
        case VertexBufferElement::Type::IVEC4:
            return GL_INT;
        case VertexBufferElement::Type::FLOAT:
        case VertexBufferElement::Type::VEC2:
        case VertexBufferElement::Type::VEC3:
        case VertexBufferElement::Type::VEC4:
        case VertexBufferElement::Type::MAT3:
        case VertexBufferElement::Type::MAT4:
            return GL_FLOAT;
        default:
            break;
    }
    ASSERT(false, "Unknown vertex buffer element type");
}

} // namespace atta::graphics::gl