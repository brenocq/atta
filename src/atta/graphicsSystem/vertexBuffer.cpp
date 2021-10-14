//--------------------------------------------------
// Atta Graphics System
// vertexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/vertexBuffer.h>

namespace atta
{
    VertexBufferElement::VertexBufferElement(std::string name_, Type type_, bool normalized_):
        name(name_), type(type_), size(sizeFromType(type_)), offset(0), normalized(normalized_)
    {

    }

    uint32_t VertexBufferElement::getComponentCount() const
    {
        switch(type)
        {
            case Type::BOOL: 
            case Type::INT: 
            case Type::UINT: 
            case Type::FLOAT: return 1;
            case Type::VEC2: 
            case Type::IVEC2: return 2;
            case Type::VEC3:
            case Type::IVEC3: return 3;
            case Type::VEC4: 
            case Type::IVEC4: return 4;
            case Type::MAT3: return 3*3;
            case Type::MAT4:return 4*4;
        }
        ASSERT(false, "Unknown vertex buffer element type");
    }

    uint32_t VertexBufferElement::sizeFromType(Type type) const
    {
        switch(type)
        {
            case Type::BOOL: return 1;
            case Type::INT: 
            case Type::UINT: 
            case Type::FLOAT: return 4;
            case Type::VEC2: 
            case Type::IVEC2: return 4 * 2;
            case Type::VEC3:
            case Type::IVEC3: return 4 * 3;
            case Type::VEC4: 
            case Type::IVEC4: return 4 * 4;
            case Type::MAT3: return 3*3 * 4;
            case Type::MAT4:return 4*4 * 4;
        }
        ASSERT(false, "Could not calculate size from element type");
    }

    VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements):
        _elements(elements)
    {
        calculateOffsetsAndStride();
    }

    void VertexBufferLayout::calculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        _stride = 0;
        for(auto& element : _elements)
        {
            element.offset = offset;
            offset += element.size;
            _stride += element.size;
        }
    }

    VertexBuffer::VertexBuffer(const CreateInfo& info):
        _layout(info.layout), _usage(info.usage), _size(info.size), _data(info.data)
    {
    }
}
