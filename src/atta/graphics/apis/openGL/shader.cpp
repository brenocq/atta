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

namespace atta::graphics::gl {

Shader::Shader(const fs::path& file) : gfx::Shader(file), _id(0) {
    // Create shaders (they are compiled at creation)
    // for (auto shaderPath : info.shaderPaths) {
    //    // Create shader
    //    Shader::CreateInfo info{};
    //    info.filepath = shaderPath;
    //    std::shared_ptr<Shader> shader = std::make_shared<Shader>(info);

    //    // Get shader texture units
    //    if (shader->getTextureUnits().size() > 0) {
    //        const std::vector<std::string>& textureUnits = shader->getTextureUnits();
    //        _textureUnits.insert(_textureUnits.end(), textureUnits.begin(), textureUnits.end());
    //    }

    //    // Save shader
    //    _shaders.push_back(std::static_pointer_cast<Shader>(shader));
    //}

    // compile();
}

Shader::~Shader() {
    if (_id > 0) {
        glDeleteProgram(_id);
        _id = 0;
    }
}

void Shader::preprocessICode() {}

void Shader::compile() {
    if (_id > 0) {
        glDeleteProgram(_id);
        _id = 0;
    }

    // Recompile shaders
    // for (auto& shader : _shaders)
    //    shader->compile();

    //_id = glCreateProgram();

    //// Attach shaders
    // for (auto shader : _shaders)
    //     glAttachShader(_id, std::static_pointer_cast<Shader>(shader)->getHandle());

    //// Link shaders
    // glLinkProgram(_id);

    // int success;
    // char infoLog[512];
    // glGetProgramiv(_id, GL_LINK_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(_id, 512, NULL, infoLog);
    //     std::vector<std::string> shaderPaths;
    //     for (auto shader : _shaders)
    //         shaderPaths.push_back(fs::absolute(shader->getFilepath()).string());
    //     ASSERT(false, "Failed to link shader group [w]$0[]:\n[*w]Shaders:[w]$1\n[*w]Error:[w]\n$2", _debugName, shaderPaths, infoLog);
    // }

    //// Detach and delete shaders
    // for (auto shader : _shaders) {
    //     std::shared_ptr<Shader> s = std::static_pointer_cast<Shader>(shader);
    //     glDetachShader(_id, s->getHandle());
    // }
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

//------------------------------------------------//
//---------- Atta to OpenGL conversions ----------//
//------------------------------------------------//
// GLenum Shader::convertFileToShaderType(const fs::path& filepath) {
//    std::string extension = filepath.extension().string();
//    if (extension == ".vert")
//        return GL_VERTEX_SHADER;
//    if (extension == ".frag")
//        return GL_FRAGMENT_SHADER;
//    if (extension == ".geom")
//        return GL_GEOMETRY_SHADER;
//    ASSERT(false, "Unknown shader file format [w]$0[]. Instead of [*w]$1[], it should be [w].vert[], [w].frag[], or [w].geom[]", filepath.string(),
//           extension);
//    return 0;
//}

} // namespace atta::graphics::gl