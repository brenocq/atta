//--------------------------------------------------
// Atta Graphics Module
// bufferLayout.cpp
// Date: 2023-07-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/bufferLayout.h>

namespace atta::graphics {

//-------------------------------------------//
//---------- BufferLayout::Element ----------//
//-------------------------------------------//
std::vector<std::string> toStr = {"NONE",  "bool",  "int",   "uint", "float", "vec2",      "vec3",       "vec4",
                                  "ivec2", "ivec3", "ivec4", "mat3", "mat4",  "sampler2D", "samplerCube"};
BufferLayout::Element::Type BufferLayout::Element::typeFromString(std::string type) {
    for (int i = 0; i < toStr.size(); i++)
        if (toStr[i] == type)
            return (Type)i;
    LOG_WARN("gfx::BufferLayout::Element", "Unknown type [w]$0[]", type);
    return Type::NONE;
}

std::string BufferLayout::Element::typeToString(Type type) { return toStr[(int)type]; }

uint32_t BufferLayout::Element::sizeFromType(Type type) {
    switch (type) {
        case Type::NONE:
            LOG_WARN("gfx::BufferLayout::Element", "Trying to get size of unknown type");
            return 0;
        case Type::BOOL:
            return 4;
        case Type::INT:
            return 4;
        case Type::UINT:
            return 4;
        case Type::FLOAT:
            return 4;
        case Type::VEC2:
            return 2 * 4;
        case Type::VEC3:
            return 3 * 4;
        case Type::VEC4:
            return 4 * 4;
        case Type::IVEC2:
            return 2 * 4;
        case Type::IVEC3:
            return 3 * 4;
        case Type::IVEC4:
            return 4 * 4;
        case Type::MAT3:
            return 3 * 3 * 4;
        case Type::MAT4:
            return 4 * 4 * 4;
        case Type::SAMPLER_2D:
            return 4;
        case Type::SAMPLER_CUBE:
            return 4;
    }
    LOG_WARN("gfx::BufferLayout::Element", "Trying to get size of unknown type");
    return 0;
}

uint32_t BufferLayout::Element::componentCountFromType(Type type) {
    switch (type) {
        case Type::BOOL:
        case Type::INT:
        case Type::UINT:
        case Type::FLOAT:
        case Type::SAMPLER_2D:
        case Type::SAMPLER_CUBE:
            return 1;
        case Type::VEC2:
        case Type::IVEC2:
            return 2;
        case Type::VEC3:
        case Type::IVEC3:
            return 3;
        case Type::VEC4:
        case Type::IVEC4:
            return 4;
        case Type::MAT3:
            return 3 * 3;
        case Type::MAT4:
            return 4 * 4;
        default:
            break;
    }
    LOG_WARN("gfx::BufferLayout::Element", "Trying to get component size of unknown type");
    return 0;
}

//----------------------------------//
//---------- BufferLayout ----------//
//----------------------------------//
BufferLayout::BufferLayout(const std::initializer_list<Element>& elements) : _elements(elements) {
    uint32_t offset = 0;
    for (auto& element : _elements) {
        element.offset = offset;
        offset += element.size;
    }
}

void BufferLayout::push(Element::Type type, std::string name) {
    if (type == Element::Type::NONE || exists(name))
        return;
    Element e;
    e.name = name;
    e.type = type;
    e.size = Element::sizeFromType(type);
    e.offset = getStride();
    _elements.push_back(e);
}

const std::vector<BufferLayout::Element>& BufferLayout::getElements() const { return _elements; }

bool BufferLayout::exists(std::string name) const {
    for (BufferLayout::Element e : _elements)
        if (e.name == name)
            return true;
    return false;
}

uint32_t BufferLayout::getElementCount() const { return static_cast<uint32_t>(_elements.size()); }

uint32_t BufferLayout::getStride() const {
    uint32_t stride = 0;
    for (auto& element : _elements)
        stride += element.size;
    return stride;
}

} // namespace atta::graphics
