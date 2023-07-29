//--------------------------------------------------
// Atta Graphics Module
// shader.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_SHADER_H
#define ATTA_GRAPHICS_APIS_OPENGL_SHADER_H

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/image.h>
#include <atta/graphics/shader.h>

namespace atta::graphics::gl {

class Shader final : public gfx::Shader {
  public:
    Shader(const fs::path& file);
    ~Shader();

    std::string generateApiCode(ShaderType type, std::string iCode) override;
    void compile() override;
    void bind() override;

    void setBool(const char* name, bool b) override;
    void setInt(const char* name, int i) override;
    void setFloat(const char* name, float f) override;
    void setVec2(const char* name, vec2 v) override;
    void setVec3(const char* name, vec3 v) override;
    void setVec4(const char* name, vec4 v) override;
    void setMat3(const char* name, mat3 m) override;
    void setMat4(const char* name, mat4 m) override;
    void setImage(const char* name, StringId sid) override;
    void setImage(const char* name, std::shared_ptr<gfx::Image> image) override;
    void setCubemap(const char* name, StringId sid) override;
    void setCubemap(const char* name, std::shared_ptr<gfx::Image> image) override;

    inline unsigned int getLoc(const char* name);

    OpenGLId getHandle() const;
    void deleteShader();

  private:
    OpenGLId _id;
    std::vector<std::string> _textureUnits;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_SHADER_H
