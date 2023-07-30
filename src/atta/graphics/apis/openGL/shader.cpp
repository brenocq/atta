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
    // LOG_DEBUG("gfx::gl::Shader", "Generating OpenGL code [y]$1[]...\n$0", iCode, type);
    iCode = "#version 300 es\n"
            "precision mediump float;\n" +
            iCode;

    switch (type) {
        case ShaderType::VERTEX: {
            // Solve OUT_VERTEX
            iCode = std::regex_replace(iCode, std::regex("OUT_VERTEX"), "gl_Position");
            // Solve POINT_SIZE
            iCode = std::regex_replace(iCode, std::regex("POINT_SIZE"), "gl_PointSize");
            break;
        }
        case ShaderType::GEOMETRY: {
            // Solve LAYER
            iCode = std::regex_replace(iCode, std::regex("LAYER"), "gl_Layer");
            break;
        }
        case ShaderType::FRAGMENT: {
            // Solve DEPTH
            iCode = std::regex_replace(iCode, std::regex("DEPTH"), "gl_FragDepth");
            break;
        }
    }
    // LOG_DEBUG("gfx::gl::Shader", "Final code is\n$0", iCode);
    return iCode;
}

void Shader::compile() {
    if (_id > 0) {
        glDeleteProgram(_id);
        _id = 0;
    }

    std::vector<OpenGLId> shaderIds;
    for (auto [type, shaderCode] : _shaderCodes) {
        // LOG_DEBUG("Shader", "Compile $0", type);
        // Create shader
        OpenGLId id = glCreateShader(convertToShaderType(type));
        shaderIds.push_back(id);

        // TODO Extract texture units from fragment shader

        // Compile
        const char* code = shaderCode.apiCode.c_str();
        // LOG_DEBUG("gfx::gl::Shader", "Code is -------------->\n$0", code);
        glShaderSource(id, 1, &code, NULL);
        glCompileShader(id);

        // Check errors
        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            LOG_ERROR("gfx::gl::Shader", "Failed to compile shader [w]$0[] type [w]$1[]. Error: [w]$2", _file.string(), type, infoLog);
        }

        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);
        //// color attribute
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // glEnableVertexAttribArray(1);
        //// texture coord attribute
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        // glEnableVertexAttribArray(2);
    }

    _id = glCreateProgram();

    // Attach shaders
    for (OpenGLId shaderId : shaderIds)
        glAttachShader(_id, shaderId);

    // Link shaders
    glLinkProgram(_id);

    int success;
    char infoLog[512];
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        LOG_ERROR("gfx::gl::Shader", "Failed to link shader [w]$0[]. Error: [w]$1", _file.string(), infoLog);
    }

    // Detach and delete shaders
    for (OpenGLId id : shaderIds) {
        glDetachShader(_id, id);
        glDeleteShader(id);
    }
}

void Shader::bind() { glUseProgram(_id); }

void Shader::setBool(const char* name, bool b) { glUniform1ui(getLoc(name), (unsigned int)b); }

void Shader::setInt(const char* name, int i) { glUniform1i(getLoc(name), i); }

void Shader::setFloat(const char* name, float f) { glUniform1f(getLoc(name), f); }

void Shader::setVec2(const char* name, vec2 v) { glUniform2fv(getLoc(name), 1, &v.x); }

void Shader::setVec3(const char* name, vec3 v) { glUniform3fv(getLoc(name), 1, &v.x); }

void Shader::setVec4(const char* name, vec4 v) { glUniform4fv(getLoc(name), 1, &v.x); }

void Shader::setMat3(const char* name, mat3 m) { glUniformMatrix3fv(getLoc(name), 1, GL_FALSE, m.data); }

void Shader::setMat4(const char* name, mat4 m) { glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, m.data); }

void Shader::setImage(const char* name, StringId sid) {
    // std::shared_ptr<OpenGLAPI> renderer = std::static_pointer_cast<OpenGLAPI>(graphics::getGraphicsAPI());
    // std::shared_ptr<Image> image = renderer->getOpenGLImages()[sid.getId()];
    // static std::map<StringId, bool> lastWarns; // Used to avoid spamming warn

    // if (!image) {
    //     if (!lastWarns[sid])
    //         LOG_WARN("gfx::gl::Shader", "(setImage) Trying to use image that was never loaded: $0 = \"$1\"", name, sid);
    //     lastWarns[sid] = true;
    //     return;
    // }
    // lastWarns[sid] = false;

    // int imgUnit = -1;
    // for (unsigned i = 0; i < _textureUnits.size(); i++)
    //     if (_textureUnits[i] == name) {
    //         imgUnit = int(i) + 1;
    //         break;
    //     }

    // if (imgUnit == -1) {
    //     LOG_WARN("gfx::gl::Shader", "(setImage) Trying to set texture [w]$0[], that was not found in the fragment shader code", name);
    //     return;
    // }

    //// Ensure image is set to write texture unit
    // GLint imageLocation = glGetUniformLocation(_id, name);
    // glUniform1i(imageLocation, imgUnit);

    //// Activate texture unit
    // glActiveTexture(GL_TEXTURE0 + imgUnit);
    // glBindTexture(GL_TEXTURE_2D, image->getHandle());
}

void Shader::setImage(const char* name, std::shared_ptr<gfx::Image> inImage) {
    // std::shared_ptr<Image> image = std::static_pointer_cast<Image>(inImage);

    // if (!image) {
    //     LOG_WARN("gfx::gl::Shader", "(setImage) Trying to set [w]$0[] with image that was never created", name);
    //     return;
    // }

    // int imgUnit = -1;
    // for (unsigned i = 0; i < _textureUnits.size(); i++)
    //     if (_textureUnits[i] == name) {
    //         imgUnit = int(i) + 1;
    //         break;
    //     }

    // if (imgUnit == -1) {
    //     LOG_WARN("gfx::gl::Shader", "(setImage) Trying to set texture [w]$0[], that was not found in the fragment shader code", name);
    //     return;
    // }

    //// Ensure image is set to write texture unit
    // GLint imageLocation = glGetUniformLocation(_id, name);
    // glUniform1i(imageLocation, imgUnit);

    //// Activate texture unit
    // glActiveTexture(GL_TEXTURE0 + imgUnit);
    // glBindTexture(GL_TEXTURE_2D, image->getHandle());
}

void Shader::setCubemap(const char* name, StringId sid) {
    std::shared_ptr<OpenGLAPI> renderer = std::static_pointer_cast<OpenGLAPI>(graphics::getGraphicsAPI());
    if (renderer->getOpenGLCubemaps().find(sid.getId()) == renderer->getOpenGLCubemaps().end()) {
        LOG_WARN("gfx::gl::Shader", "(setCubemap) Trying to use cubemap that was never generated: $0 = \"$1\"", name, sid);
        return;
    }
    OpenGLId cubemap = renderer->getOpenGLCubemaps()[sid.getId()];

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
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    // LOG_DEBUG("gfx::gl::Shader", "Bind $0 to texture unit $1, id $2 -> $3", name, imgUnit, cubemap, _textureUnits);
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

// Shader::Shader(const Shader::CreateInfo& info) : gfx::Shader(info), _id(0) { recompile(); }

// Shader::~Shader() {
//     if (!_id)
//         deleteShader();
// }

// void Shader::recompile() {
// if (!_id)
//     deleteShader();

//// Create shader
//_id = glCreateShader(convertFileToShaderType(_filepath));

//// Read file
// std::stringstream fileSS;
// fs::path absolutePath = file::solveResourcePath(_filepath);
// std::ifstream file(absolutePath);
// fileSS << file.rdbuf();
// file.close();

//// Extract texture units from fragment shader
////if (convertFileToShaderType(_filepath) == GL_FRAGMENT_SHADER)
////    extractTextureUnits(fileSS);

//// Compile
// std::string codeStr = fileSS.str();
// const char* code = codeStr.c_str();
// glShaderSource(_id, 1, &code, NULL);
// glCompileShader(_id);

//// Check errors
// int success;
// char infoLog[512];
// glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
// if (!success) {
//     glGetShaderInfoLog(_id, 512, NULL, infoLog);
//     ASSERT(false, "Failed to compile shader [*w]$0[]:[w]\n$1\ncode->$2", fs::absolute(_filepath).string(), infoLog, code);
// }
//}

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
