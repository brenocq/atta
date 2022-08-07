//--------------------------------------------------
// Atta Graphics Module
// shader.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SHADER_H
#define ATTA_GRAPHICS_SHADER_H
#include <atta/core/stringId.h>
#include <atta/graphics/shaderUniform.h>

namespace atta::graphics {
class Shader {
  public:
    struct CreateInfo {
        fs::path filepath;
    };

    Shader(const CreateInfo& info);
    virtual ~Shader() = default;

    virtual void recompile() = 0;
    fs::path getFilepath() const { return _filepath; }

  protected:
    fs::path _filepath;
    std::unordered_map<std::string, ShaderUniform> _uniforms;
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_SHADER_H
