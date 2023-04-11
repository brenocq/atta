//--------------------------------------------------
// Atta Graphics Module
// shader.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_SHADER_H
#define ATTA_GRAPHICS_APIS_OPENGL_SHADER_H

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/shader.h>

namespace atta::graphics::gl {

class Shader final : public gfx::Shader {
  public:
    Shader(const Shader::CreateInfo& info);
    ~Shader();

    void recompile() override;

    OpenGLId getHandle() const { return _id; }
    std::vector<std::string> getTextureUnits() { return _textureUnits; }
    void deleteShader();

    static GLenum convertFileToShaderType(const fs::path& filepath);

  private:
    void extractTextureUnits(const std::stringstream& sstream);

    OpenGLId _id;
    std::vector<std::string> _textureUnits;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_SHADER_H
