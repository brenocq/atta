//--------------------------------------------------
// Atta Graphics Module
// shader.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SHADER_H
#define ATTA_GRAPHICS_SHADER_H
#include <atta/graphics/bufferLayout.h>
#include <atta/graphics/image.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Shader {
  public:
    Shader(const fs::path& file);
    virtual ~Shader() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void setBool(const char* name, bool b) = 0;
    virtual void setInt(const char* name, int i) = 0;
    virtual void setFloat(const char* name, float f) = 0;
    virtual void setVec2(const char* name, vec2 v) = 0;
    virtual void setVec3(const char* name, vec3 v) = 0;
    virtual void setVec4(const char* name, vec4 v) = 0;
    virtual void setMat3(const char* name, mat3 m) = 0;
    virtual void setMat4(const char* name, mat4 m) = 0;

    fs::path getFile() const;
    const BufferLayout& getVertexBufferLayout() const;
    const BufferLayout& getPerFrameLayout() const;
    const BufferLayout& getPerDrawLayout() const;
    const BufferLayout& getPerFrameImageLayout() const;
    const BufferLayout& getPerDrawImageLayout() const;

    /**
     * @brief Shader type
     */
    enum ShaderType { VERTEX = 0, GEOMETRY, FRAGMENT };

  protected:
    struct ShaderCode {
        std::string iCode;   ///< Graphics API independent code (intermediate code)
        std::string apiCode; ///< Graphics API specific code
    };

    /**
     * @brief Process intermediate code to generate API specific code.
     *
     * It will mainly substitute placeholders with Graphics API specific code.
     *
     * @param type Shader type
     * @param iCode Intermediate code
     *
     * @return API specific code
     */
    virtual std::string generateApiCode(ShaderType type, std::string iCode) = 0;

    /**
     * @brief Compile graphics API specific code
     */
    virtual void compile() = 0;

  private:
    /**
     * @brief Process .asl file to generate ICode
     *
     * The preprocessing phase consists of a number of operations:
     *  - Entrypoints are detected (vertex, geometry, fragment)
     *  - Default vertex and fragment are added to the shader if not specified by user.
     *  - Placeholders for graphics API specific code are added.
     *
     * The preprocessed ASL is called ASL ICode (ASL Intermediate Code). After the ASL ICode is generated, ASL ICode is further divided in
     * api-specific ICode (VERTEX, GEOMETRY, FRAGMENT).
     */
    void processASL();

    /**
     * @brief Remove comments from code
     *
     * @param code ASL Code
     *
     * @return Code without comments
     */
    std::string removeComments(std::string code);

    /**
     * @brief Replace defines
     *
     * @param code ASL Code
     *
     * @return Code with applied defines
     */
    std::string replaceDefines(std::string code);

    /**
     * @brief Check which entrypoints were defined
     *
     * Entrypoints can be VERTEX, GEOMETRY, FRAGMENT
     *
     * @param code ASL Code
     *
     * @return Set with defined entrypoints
     */
    std::set<ShaderType> detectEntrypoints(std::string code);

    /**
     * @brief Generate type-specific ICode (Intermediate Code) from ASL code
     *
     * @param type ICode shader type to be generated
     * @param aslCode ASL code
     *
     * @return type specific ICode
     */
    std::string generateICode(ShaderType type, std::string aslCode);

    /**
     * @brief Remove unused functions from code
     *
     * Initially, all functions functions are detected and marked as unused. After that, the used in main() and used by used functions are marked as
     * used.
     *
     * @param code Code
     *
     * @return Code without unused functions
     */
    std::string removeUnusedFunctions(std::string code);

    /// Parse custom types (structs) from _aslCode and populate _customTypes
    void parseCustomTypes();

    /// Process vertex input from _aslCode and populate _vertexLayout
    void populateVertexLayout();

    /// Process fragment output from _aslCode and populate _fragmentLayout
    void populateFragmentLayout();

    /// Go through all shader codes to extract variables and images to populate perDraw and perFrame layouts
    void populateDescriptorLayouts();

  protected:
    fs::path _file;
    std::string _aslCode;
    std::map<std::string, BufferLayout> _customTypes; ///< Custom types extracted from _aslCode

    /*
     * @brief Store types and names stored in the layout
     *
     * This is necessary because we lose the struct and array information when populating the BufferLayout, which is the flat version of the
     * layout
     */
    struct LayoutMember {
        LayoutMember(std::string type_, std::string name_);
        LayoutMember(std::string type_, std::string name_, size_t arraySize_);
        std::string type;
        std::string name;
        bool isArray;
        size_t arraySize;
    };
    std::vector<LayoutMember> _perFrameLayoutMembers;
    std::vector<LayoutMember> _perDrawLayoutMembers;

    BufferLayout _vertexLayout;        ///< Vertex layout from vertex shader input
    BufferLayout _fragmentLayout;      ///< Output attachments from fragment shader
    BufferLayout _perFrameLayout;      ///< Buffer layout from perFrame variables
    BufferLayout _perDrawLayout;       ///< Buffer layout from perDraw variables
    BufferLayout _perFrameImageLayout; ///< Image layout from perFrame textures
    BufferLayout _perDrawImageLayout;  ///< Image layout from perDraw textures
    std::map<ShaderType, ShaderCode> _shaderCodes;
};

inline std::ostream& operator<<(std::ostream& os, Shader::ShaderType shaderType) {
    static std::array<const char*, 3> names = {"VERTEX", "GEOMETRY", "FRAGMENT"};
    os << names[(int)shaderType];
    return os;
}

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_SHADER_H
