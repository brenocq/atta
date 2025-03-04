//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/manager.h>
#include <atta/graphics/apis/openGL/openGLAPI.h>
#include <atta/graphics/apis/openGL/shader.h>
#include <atta/graphics/interface.h>
#include <regex>

namespace atta::graphics::gl {

Shader::Shader(const fs::path& file) : gfx::Shader(file), _id(0) {
    for (auto& [type, shaderCode] : _shaderCodes)
        shaderCode.apiCode = generateApiCode(type, shaderCode.iCode);
    compile();
}

Shader::~Shader() {
    if (_id > 0) {
        glDeleteProgram(_id);
        _id = 0;
    }
}

std::string Shader::generateApiCode(ShaderType type, std::string iCode) {
    uint32_t openGLVersion = gfx::getGraphicsAPI()->getAPIVersion();
    std::string apiCode;

    if (openGLVersion >= 410) {
        // macOS (OpenGL Core 4.1)
        apiCode = "#version 410 core\n";
    } else if (openGLVersion >= 300 && openGLVersion < 400) {
        // OpenGL ES (Linux, Android, Web)
        apiCode = "#version 300 es\n"
                  "precision mediump float;\n";
    } else {
        // Default fallback (use OpenGL 3.0 if nothing else works)
        apiCode = "#version 300 core\n";
    }
    apiCode += iCode;

    // Replace perFrame/perDraw
    apiCode = std::regex_replace(apiCode, std::regex(R"(\b(perFrame|perDraw))"), "uniform");

    // Replace perVertex declaration from iCode
    if (type == VERTEX)
        apiCode = std::regex_replace(apiCode, std::regex(R"(\bperVertex)"), "out");
    else if (type == FRAGMENT)
        apiCode = std::regex_replace(apiCode, std::regex(R"(\bperVertex)"), "in");

    // Populate texture units
    std::regex regexTexture(R"(\s*uniform\s+(sampler2D|samplerCube)\s+(\w+)\s*;)");
    std::smatch match;
    auto start = apiCode.cbegin();
    while (std::regex_search(start, apiCode.cend(), match, regexTexture)) {
        std::string type = match[1].str();
        std::string name = match[2].str();

        // Add texture unit
        bool found = false;
        for (size_t i = 0; i < _textureUnits.size(); i++)
            if (_textureUnits[i] == name) {
                found = true;
                break;
            }
        if (!found)
            _textureUnits.push_back(name);

        // Move to the next match
        start = match.suffix().first;
    }
    // LOG_DEBUG("gfx::gl::Shader", "Generated API code for shader [g]$1[]\n[w]$0[]", apiCode, _file.string());

    return apiCode;
}

void Shader::compile() {
    if (_id > 0) {
        glDeleteProgram(_id);
        _id = 0;
    }

    std::vector<OpenGLId> shaderIds;
    for (auto [type, shaderCode] : _shaderCodes) {
        // Create shader
        OpenGLId id = glCreateShader(convertToShaderType(type));
        shaderIds.push_back(id);

        // Compile
        const char* code = shaderCode.apiCode.c_str();
        glShaderSource(id, 1, &code, NULL);
        glCompileShader(id);

        // Get log
        int logLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(id, logLength, NULL, log.data());

        // Check errors
        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            LOG_ERROR("gfx::gl::Shader", "Failed to compile shader [w]$0[] type [w]$1[] with error: \n[w]$2", _file.string(), type, log.data());
        } else {
            // Check warnings if no errors
            if (logLength > 1) {
                // Log may contain warnings even if compilation succeeds
                LOG_WARN("gfx::gl::Shader", "Shader [w]$0[] type [w]$1[] compiled with messages: \n[w]$2", _file.string(), type, log.data());
            }
        }
    }

    _id = glCreateProgram();

    // Attach shaders
    for (OpenGLId shaderId : shaderIds)
        glAttachShader(_id, shaderId);

    // Link shaders
    glLinkProgram(_id);

    int success;
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        int logLength = 0;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(_id, logLength, NULL, log.data());
        LOG_ERROR("gfx::gl::Shader", "Failed to link shader [w]$0[] with error: \n[w]$1", _file.string(), log.data());
    }

    // Detach and delete shaders
    for (OpenGLId id : shaderIds) {
        glDetachShader(_id, id);
        glDeleteShader(id);
    }
}

void Shader::bind() { glUseProgram(_id); }

void Shader::unbind() {}

void Shader::setBool(const char* name, bool b) { glUniform1ui(getLoc(name), (unsigned int)b); }

void Shader::setInt(const char* name, int i) { glUniform1i(getLoc(name), i); }

void Shader::setFloat(const char* name, float f) { glUniform1f(getLoc(name), f); }

void Shader::setVec2(const char* name, vec2 v) { glUniform2fv(getLoc(name), 1, &v.x); }

void Shader::setVec3(const char* name, vec3 v) { glUniform3fv(getLoc(name), 1, &v.x); }

void Shader::setVec4(const char* name, vec4 v) { glUniform4fv(getLoc(name), 1, &v.x); }

void Shader::setMat3(const char* name, mat3 m) {
    mat3 mt = transpose(m);
    glUniformMatrix3fv(getLoc(name), 1, GL_FALSE, mt.data);
}

void Shader::setMat4(const char* name, mat4 m) {
    mat4 mt = transpose(m);
    glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, mt.data);
}

void Shader::setImage(const char* name, std::shared_ptr<gfx::Image> inImage) {
    std::shared_ptr<Image> image = std::static_pointer_cast<Image>(inImage);

    if (!image) {
        LOG_WARN("gfx::gl::Shader", "(setImage) Trying to set [w]$0[] with image that was never created", name);
        return;
    }

    int imgUnit = -1;
    for (unsigned i = 0; i < _textureUnits.size(); i++)
        if (_textureUnits[i] == name) {
            imgUnit = int(i) + 1;
            break;
        }

    if (imgUnit == -1) {
        LOG_WARN("gfx::gl::Shader", "(setImage) Trying to set texture [w]$0[], that was not found in the fragment shader code", name);
        return;
    }

    // Ensure image is set to write texture unit
    GLint imageLocation = glGetUniformLocation(_id, name);
    glUniform1i(imageLocation, imgUnit);

    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + imgUnit);
    glBindTexture(GL_TEXTURE_2D, image->getHandle());
}

void Shader::setCubemap(const char* name, std::shared_ptr<gfx::Image> inImage) {
    std::shared_ptr<Image> image = std::static_pointer_cast<Image>(inImage);

    if (!image) {
        LOG_WARN("gfx::gl::Shader", "(setCubemap) Trying to set [w]$0[] with image that was never created", name);
        return;
    }

    if (!image->isCubemap()) {
        LOG_WARN("gfx::gl::Shader", "(setCubemap) Trying to set [w]$0[] with image that is not a cubemap", name);
        return;
    }

    int imgUnit = -1;
    for (unsigned i = 0; i < _textureUnits.size(); i++)
        if (_textureUnits[i] == name) {
            imgUnit = int(i) + 1;
            break;
        }

    if (imgUnit == -1) {
        LOG_WARN("gfx::gl::Shader", "(setCubemap) Trying to set cubemap [w]$0[], that was not found in the fragment shader code", name);
        return;
    }

    // Ensure image is set to write texture unit
    GLint imageLocation = glGetUniformLocation(_id, name);
    glUniform1i(imageLocation, imgUnit);

    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + imgUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, image->getHandle());
}

unsigned int Shader::getLoc(const char* name) { return glGetUniformLocation(_id, name); }

OpenGLId Shader::getHandle() const { return _id; }

void Shader::deleteShader() {
    glDeleteShader(_id);
    _id = 0;
}

unsigned int Shader::convertToShaderType(ShaderType type) {
    switch (type) {
        case ShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderType::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case ShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        default:
            LOG_WARN("gfx::gl::Shader", "Failed to convert shader type");
            return 0;
    }
    return 0;
}

} // namespace atta::graphics::gl
