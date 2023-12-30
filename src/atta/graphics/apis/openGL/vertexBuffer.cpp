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

        uint32_t componentCount = BufferLayout::Element::componentCountFromType(element.type);

        // Define attribute format
        if (openGLType == GL_INT)
            glVertexAttribIPointer(i, componentCount, openGLType, _layout.getStride(), reinterpret_cast<void*>(element.offset));
        else
            glVertexAttribPointer(i, componentCount, openGLType, GL_FALSE, _layout.getStride(), reinterpret_cast<void*>(element.offset));

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

void VertexBuffer::update(const uint8_t* data, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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

GLenum VertexBuffer::convertBaseType(BufferLayout::Element::Type type) {
    switch (type) {
        case BufferLayout::Element::Type::BOOL:
            return GL_BOOL;
        case BufferLayout::Element::Type::INT:
        case BufferLayout::Element::Type::UINT:
        case BufferLayout::Element::Type::IVEC2:
        case BufferLayout::Element::Type::IVEC3:
        case BufferLayout::Element::Type::IVEC4:
            return GL_INT;
        case BufferLayout::Element::Type::FLOAT:
        case BufferLayout::Element::Type::VEC2:
        case BufferLayout::Element::Type::VEC3:
        case BufferLayout::Element::Type::VEC4:
        case BufferLayout::Element::Type::MAT3:
        case BufferLayout::Element::Type::MAT4:
            return GL_FLOAT;
        default:
            break;
    }
    ASSERT(false, "Unknown vertex buffer element type");
}

} // namespace atta::graphics::gl
