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
std::vector<std::string> toStr = {"?",     "bool",  "int",   "uint", "float", "vec2",      "vec3",       "vec4",
                                  "ivec2", "ivec3", "ivec4", "mat3", "mat4",  "sampler2D", "samplerCube"};
BufferLayout::Element::Type BufferLayout::Element::typeFromString(std::string type) {
    for (size_t i = 0; i < toStr.size(); i++)
        if (toStr[i] == type)
            return (Type)i;
    return Type::NONE;
}

std::string BufferLayout::Element::typeToString(Type type) { return toStr[(int)type]; }

uint32_t BufferLayout::Element::sizeFromType(AlignmentType alignmentType, Type type) {
    switch (alignmentType) {
        case AlignmentType::DEFAULT: {
            switch (type) {
                case Type::BOOL:
                    return 1;
                case Type::INT:
                case Type::UINT:
                case Type::FLOAT:
                case Type::SAMPLER_2D:
                case Type::SAMPLER_CUBE:
                    return 4;
                case Type::VEC2:
                case Type::IVEC2:
                    return 2 * 4;
                case Type::VEC3:
                case Type::IVEC3:
                    return 3 * 4;
                case Type::VEC4:
                case Type::IVEC4:
                    return 4 * 4;
                case Type::MAT3:
                    return 3 * 3 * 4;
                case Type::MAT4:
                    return 4 * 4 * 4;
                default:
                    LOG_WARN("gfx::BufferLayout::Element", "Trying to get size of unknown type");
                    return 0;
            }
        }
        case AlignmentType::STD140:
        case AlignmentType::STD430: {
            switch (type) {
                case Type::BOOL:
                case Type::INT:
                case Type::UINT:
                case Type::FLOAT:
                case Type::SAMPLER_2D:
                case Type::SAMPLER_CUBE:
                    return 4;
                case Type::VEC2:
                case Type::IVEC2:
                    return 2 * 4;
                case Type::VEC3:
                case Type::IVEC3:
                    return 3 * 4;
                case Type::VEC4:
                case Type::IVEC4:
                    return 4 * 4;
                case Type::MAT3:
                    return 3 * 4 * 4;
                case Type::MAT4:
                    return 4 * 4 * 4;
                default:
                    LOG_WARN("gfx::BufferLayout::Element", "Trying to get size of unknown type");
                    return 0;
            }
        }
    }
    return 0;
}

uint32_t BufferLayout::Element::alignmentFromType(AlignmentType alignmentType, Type type) {
    switch (alignmentType) {
        case AlignmentType::DEFAULT: {
            switch (type) {
                case Type::BOOL:
                case Type::SAMPLER_2D:
                case Type::SAMPLER_CUBE:
                    return 1;
                case Type::INT:
                case Type::UINT:
                case Type::FLOAT:
                case Type::VEC2:
                case Type::VEC3:
                case Type::VEC4:
                case Type::IVEC2:
                case Type::IVEC3:
                case Type::IVEC4:
                case Type::MAT3:
                case Type::MAT4:
                    return 4;
                default:
                    LOG_WARN("gfx::BufferLayout::Element", "Trying to get alignment of unknown type");
                    return 0;
            }
        }
        case AlignmentType::STD140:
        case AlignmentType::STD430: {
            switch (type) {
                case Type::SAMPLER_2D:
                case Type::SAMPLER_CUBE:
                    return 1;
                case Type::BOOL:
                case Type::INT:
                case Type::UINT:
                case Type::FLOAT:
                    return 4;
                case Type::VEC2:
                case Type::IVEC2:
                    return 2 * 4;
                case Type::VEC3:
                case Type::IVEC3:
                case Type::VEC4:
                case Type::IVEC4:
                case Type::MAT3:
                case Type::MAT4:
                    return 4 * 4;
                default:
                    LOG_WARN("gfx::BufferLayout::Element", "Trying to get alignment of unknown type");
                    return 0;
            }
        }
    }
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
BufferLayout::BufferLayout(AlignmentType alignmentType) : _alignmentType(alignmentType), _forceNextAlign(0) {}

void BufferLayout::setAlignmentType(AlignmentType alignmentType) { _alignmentType = alignmentType; }

void BufferLayout::push(Element::Type type, std::string name, uint32_t customAlign) {
    if (type == Element::Type::NONE || exists(name))
        return;
    Element e{};
    e.name = name;
    e.type = type;
    e.size = Element::sizeFromType(_alignmentType, type);

    // Calculate offset in buffer
    if (_elements.empty())
        e.offset = 0;
    else {
        e.align = std::max(customAlign, _forceNextAlign);
        e.align = e.align ? e.align : Element::alignmentFromType(_alignmentType, type);
        _forceNextAlign = 0;
        uint32_t offset = _elements.back().offset + _elements.back().size;
        e.offset = (offset + e.align - 1) & ~(e.align - 1);
    }

    _elements.push_back(e);
}

void BufferLayout::pushStructArrayAlign() {
    if (_alignmentType == AlignmentType::STD140 || _alignmentType == AlignmentType::STD430)
        _forceNextAlign = 16;
}

const std::vector<BufferLayout::Element>& BufferLayout::getElements() const { return _elements; }

bool BufferLayout::exists(std::string name) const {
    for (BufferLayout::Element e : _elements)
        if (e.name == name)
            return true;
    return false;
}

uint32_t BufferLayout::getAlignment() const {
    uint32_t bufferAlignment = 0;
    for (auto& element : _elements)
        bufferAlignment = std::max(bufferAlignment, Element::alignmentFromType(_alignmentType, element.type));
    return bufferAlignment;
}

uint32_t BufferLayout::getStride() const {
    if (_elements.empty())
        return 0;

    // Get buffer alignment (largest type alignment)
    uint32_t bufferAlignment = getAlignment();

    // Align buffer
    uint32_t stride = _elements.back().offset + _elements.back().size;
    stride = (stride + bufferAlignment - 1) & ~(bufferAlignment - 1);

    return stride;
}

bool BufferLayout::empty() const { return _elements.empty(); }

} // namespace atta::graphics
