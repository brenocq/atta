//--------------------------------------------------
// Atta Graphics Module
// shaderUniform.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SHADER_UNIFORM_H
#define ATTA_GRAPHICS_SHADER_UNIFORM_H

namespace atta::graphics {

class ShaderUniform final {
  public:
    enum class Type { NONE = 0, BOOL, INT, UINT, FLOAT, VEC2, VEC3, VEC4, IVEC2, IVEC3, IVEC4, MAT3, MAT4 };
    ShaderUniform() = default;
    ShaderUniform(Type type, uint32_t offset);

    Type getType() const;
    uint32_t getOffset() const;
    uint32_t getSize() const;

  private:
    Type _type;
    uint32_t _offset;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_SHADER_UNIFORM_H
