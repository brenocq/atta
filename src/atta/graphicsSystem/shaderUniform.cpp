//--------------------------------------------------
// Atta Graphics System
// shaderUniform.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/shaderUniform.h>

namespace atta
{
    ShaderUniform::ShaderUniform(std::string name, Type type, uint32_t size, uint32_t offset):
        _name(name), _type(type), _size(size), _offset(offset)
    {

    }
}
