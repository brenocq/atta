//--------------------------------------------------
// Atta Graphics Module
// shader.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SHADER_H
#define ATTA_GRAPHICS_SHADER_H
#include <atta/graphics/shaderUniform.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Shader {
  public:
    Shader(const fs::path& file);
    virtual ~Shader() = default;

    virtual void compile() = 0;
    virtual void bind() = 0;

    virtual void setBool(const char* name, bool b) = 0;
    virtual void setInt(const char* name, int i) = 0;
    virtual void setFloat(const char* name, float f) = 0;
    virtual void setVec2(const char* name, vec2 v) = 0;
    virtual void setVec3(const char* name, vec3 v) = 0;
    virtual void setVec4(const char* name, vec4 v) = 0;
    virtual void setMat3(const char* name, mat3 m) = 0;
    virtual void setMat4(const char* name, mat4 m) = 0;
    virtual void setImage(const char* name, StringId sid) = 0;
    virtual void setImage(const char* name, std::shared_ptr<Image> image) = 0;
    virtual void setCubemap(const char* name, StringId sid) = 0;
    virtual void setCubemap(const char* name, std::shared_ptr<Image> image) = 0;

    fs::path getFile() const;
    VertexBufferLayout getVertexBufferLayout() const;

  protected:
    /**
     * @brief Preprocess .asl file
     *
     * The preprocessing phase consists of a number of operations:
     *  - Entrypoints are detected (vertex, geometry, fragment)
     *  - Default vertex and fragment are added to the shader if not specified by user.
     *  - Some uppercase variables are substituted by glsl variables (VERTEX, COLOR, DEPTH, ...).
     *  - Some uppercase variables are substituted by uniform variables (PROJECTION, VIEW, MODEL, ...).
     *  - Placeholders for graphics API specific code are added.
     *
     * The preprocessed ASL is called ASL ICode (ASL Intermediate Code). After the ASL ICode is generated, ASL ICode is further divided in
     * typed-specific ICode (VERTEX, GEOMETRY, FRAGMENT).
     */
    void preprocessASL();

    /**
     * @brief Preprocess intermediate code (graphics API independent)
     *
     * Substitute placeholders with Graphics API specific code.
     */
    virtual void preprocessICode() = 0;

    /**
     * @brief Compile graphics API specific code
     */
    virtual void compile() = 0;

    fs::path _file;
    std::string _aslCode;
    std::string _iCode;

    VertexBufferLayout _vertexBufferLayout;
    std::unordered_map<std::string, ShaderUniform> _uniforms;

    enum Type { VERTEX, GEOMETRY, FRAGMENT };
    struct ShaderCode {
        Type type;
        std::string iCode; ///< Graphics API independent code (intermediate code)
        std::string code;  ///< Graphics API specific code
    };

    std::vector<ShaderCode> _shaderCodes;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_SHADER_H
