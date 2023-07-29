//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/manager.h>
#include <atta/graphics/shader.h>
#include <regex>

namespace atta::graphics {

Shader::Shader(const fs::path& file) : _file(file) {
    fs::path absolutePath = file::solveResourcePath(_file);

    // Check file
    if (!fs::exists(absolutePath)) {
        LOG_ERROR("gfx::Shader", "Shader file [w]$0[] was not found", file.string());
        return;
    }

    if (file.extension() != ".asl") {
        LOG_ERROR("gfx::Shader", "Shader file [w]$0[] does not have [w].asl[] extension", file.string());
        return;
    }

    // Read file
    std::stringstream ss;
    std::ifstream f(absolutePath);
    ss << f.rdbuf();
    f.close();
    _aslCode = ss.str();

    processASL();
}

fs::path Shader::getFile() const { return _file; }

VertexBufferLayout Shader::getVertexBufferLayout() const { return _vertexBufferLayout; }

void Shader::processASL() {
    LOG_DEBUG("gfx::Shader", "Preprocess ASL: [w]$0", _file.string());

    // Remove comments
    _aslCode = removeComments(_aslCode);

    // Add default entrypoints
    std::set<ShaderType> shaderEntrypoints = detectEntrypoints(_aslCode);
    if (shaderEntrypoints.find(VERTEX) == shaderEntrypoints.end()) {
        _aslCode += "\nvoid vertex(){ VERTEX = PROJECTION * VIEW * MODEL * vec4(VERTEX, 1.0f); }";
        shaderEntrypoints.insert(VERTEX);
    }
    if (shaderEntrypoints.find(FRAGMENT) == shaderEntrypoints.end()) {
        _aslCode += "\nvoid fragment(){ COLOR = vec4(1.0f, 0.0f, 1.0f, 1.0f); }";
        shaderEntrypoints.insert(FRAGMENT);
    }

    // Process ASL code to generate ICode and APICode
    _shaderCodes.clear();
    for (ShaderType type : shaderEntrypoints) {
        ShaderCode shaderCode;
        shaderCode.iCode = generateICode(type, _aslCode);
        shaderCode.apiCode = "";
        // LOG_DEBUG("Shader", "ICode\n[y]$0[]\nAPI Code\n[m]$1", shaderCode.iCode, shaderCode.apiCode);
        _shaderCodes[type] = shaderCode;
    }

    // Populate shader uniforms
    populateUniforms();
}

std::string Shader::removeComments(std::string code) {
    // Remove single-line comments
    code = std::regex_replace(code, std::regex("//[^\n]*"), "");
    // Remove multi-line comments
    code = std::regex_replace(code, std::regex(R"(/\*([\s\S]*?)\*/)"), "");
    return code;
}

std::set<Shader::ShaderType> Shader::detectEntrypoints(std::string code) {
    std::map<ShaderType, int> shaderOccurrences;

    // Regular expressions to match function signatures
    std::regex fragmentRegex(R"(void\s+fragment\s*\(\s*\))");
    std::regex vertexRegex(R"(void\s+vertex\s*\(\s*\))");

    // Search for the function signatures in the code and count occurrences
    for (std::sregex_iterator it(code.begin(), code.end(), fragmentRegex), end; it != end; ++it) {
        shaderOccurrences[ShaderType::FRAGMENT]++;
    }
    for (std::sregex_iterator it(code.begin(), code.end(), vertexRegex), end; it != end; ++it) {
        shaderOccurrences[ShaderType::VERTEX]++;
    }

    std::set<ShaderType> result = {};
    for (auto [type, numOccurences] : shaderOccurrences) {
        if (numOccurences == 1) {
            result.insert(type);
        } else {
            LOG_ERROR("gfx::Shader",
                      "Error found when parsing [w]$0[], there should be only one [w]$1[] entrypoint, but $2 were found. Default entrypoint will "
                      "be used instead",
                      _file.string(), type, numOccurences);
        }
    }
    return result;
}

std::string addToFuncEnd(std::string code, std::string funcName, std::string toAdd) {
    std::regex openBracketRegex("void\\s+" + funcName + "\\s*\\(\\s*\\)\\s*\\{");
    std::smatch match;
    if (std::regex_search(code, match, openBracketRegex)) {
        // Get the position of the opening curly brace of the vertex function
        size_t openBracket = match.position() + match.length();

        int nested = 1; // Counter to keep track of nested {}
        size_t insertPosition = std::string::npos;
        for (size_t i = openBracket; i < code.size(); i++) {
            if (code[i] == '{') {
                nested++;
            } else if (code[i] == '}') {
                nested--;
                if (nested == 0) {
                    // Set the insert position to the current index
                    insertPosition = i;
                    break;
                }
            }
        }
        code.insert(insertPosition - 1, "\n" + toAdd);
    }
    return code;
}

std::string Shader::generateICode(ShaderType type, std::string aslCode) {
    std::string iCode = aslCode;
    switch (type) {
        case VERTEX: {
            iCode = "in vec3 VERTEX;\n"
                    "in vec3 NORMAL;\n"
                    "in vec2 UV;\n"
                    "\n"
                    "out vec3 OUT_VERTEX;\n"
                    "out vec3 OUT_NORMAL;\n"
                    "out vec3 OUT_UV;\n"
                    "\n"
                    "uniform mat4 PROJECTION;\n"
                    "uniform mat4 VIEW;\n"
                    "uniform mat4 MODEL;\n\n" +
                    iCode;
            iCode = std::regex_replace(iCode, std::regex(R"(\bvarying)"), "out");
            iCode = addToFuncEnd(iCode, "vertex",
                                 "OUT_VERTEX = VERTEX;\n"
                                 "OUT_NORMAL = NORMAL;\n"
                                 "OUT_UV = UV;\n");
            break;
        }
        case FRAGMENT: {
            iCode = "in vec3 VERTEX;\n"
                    "in vec3 NORMAL;\n"
                    "in vec2 UV;\n"
                    "\n"
                    "out vec4 OUT_COLOR;\n"
                    "vec4 COLOR;\n\n" +
                    iCode;
            iCode = std::regex_replace(iCode, std::regex(R"(\bvarying)"), "in");
            iCode = addToFuncEnd(iCode, "fragment", "OUT_COLOR = COLOR;\n");
            break;
        }
        default:
            LOG_WARN("gfx::Shader", "Unknown type $0 when generating ICode", type);
    }
    return iCode;
}

ShaderUniform::Type Shader::convertType(std::string type) {
    if (type == "bool")
        return ShaderUniform::Type::BOOL;
    else if (type == "int")
        return ShaderUniform::Type::INT;
    else if (type == "uint")
        return ShaderUniform::Type::UINT;
    else if (type == "float")
        return ShaderUniform::Type::FLOAT;
    else if (type == "vec2")
        return ShaderUniform::Type::VEC2;
    else if (type == "vec3")
        return ShaderUniform::Type::VEC3;
    else if (type == "vec4")
        return ShaderUniform::Type::VEC4;
    else if (type == "ivec2")
        return ShaderUniform::Type::IVEC2;
    else if (type == "ivec3")
        return ShaderUniform::Type::IVEC3;
    else if (type == "ivec4")
        return ShaderUniform::Type::IVEC4;
    else if (type == "mat3")
        return ShaderUniform::Type::MAT3;
    else if (type == "mat4")
        return ShaderUniform::Type::MAT4;
    LOG_WARN("gfx::Shader", "Unknown uniform type [w]$0[] in shader [w]$1[]", type, _file.string());
    return ShaderUniform::Type::NONE;
}

void Shader::populateUniforms() {
    std::regex regex(R"(\buniform\s+(\w+)\s+(\w+)\s*;\s*)");
    std::smatch match;

    size_t offset = 0;
    _uniforms.clear();
    for (auto [type, code] : _shaderCodes) {
        auto it = code.iCode.cbegin();
        auto end = code.iCode.cend();

        while (std::regex_search(it, end, match, regex)) {
            std::string type = match[1];
            std::string name = match[2];

            if (_uniforms.find(name) == _uniforms.end()) {
                ShaderUniform::Type t = convertType(type);
                if (t != ShaderUniform::Type::NONE) {
                    ShaderUniform uniform = ShaderUniform(t, offset);
                    _uniforms[name] = uniform;
                    offset += uniform.getSize();
                }
            }

            // Move to the next match
            it = match.suffix().first;
        }
    }
}

} // namespace atta::graphics
