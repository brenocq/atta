//--------------------------------------------------
// Atta Graphics Module
// shaderUniform.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/shaderUniform.h>

namespace atta::graphics {

ShaderUniform::ShaderUniform(Type type, uint32_t offset) : _type(type), _offset(offset) {}

ShaderUniform::Type ShaderUniform::getType() const { return _type; }

uint32_t ShaderUniform::getOffset() const { return _offset; }

uint32_t ShaderUniform::getSize() const {
    switch (_type) {
        case Type::NONE:
            LOG_WARN("gfx::ShaderUniform", "Trying to get size of unknown type");
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
    }
    LOG_WARN("gfx::ShaderUniform", "Trying to get size of unknown type");
    return 0;
}

} // namespace atta::graphics
