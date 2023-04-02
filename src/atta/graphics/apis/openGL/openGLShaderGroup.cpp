//--------------------------------------------------
// Atta Graphics Module
// openGLShaderGroup.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/openGLShader.h>
#include <atta/graphics/apis/openGL/openGLShaderGroup.h>

#include <atta/graphics/interface.h>
#include <atta/graphics/apis/openGL/openGLAPI.h>

namespace atta::graphics {

OpenGLShaderGroup::OpenGLShaderGroup(const ShaderGroup::CreateInfo& info) : ShaderGroup(info), _id(0) {
    // Create shaders (they are compiled at creation)
    for (auto shaderPath : info.shaderPaths) {
        // Create shader
        Shader::CreateInfo info{};
        info.filepath = shaderPath;
        std::shared_ptr<OpenGLShader> shader = std::make_shared<OpenGLShader>(info);

        // Get shader texture units
        if (shader->getTextureUnits().size() > 0) {
            const std::vector<std::string>& textureUnits = shader->getTextureUnits();
            _textureUnits.insert(_textureUnits.end(), textureUnits.begin(), textureUnits.end());
        }

        // Save shader
        _shaders.push_back(std::static_pointer_cast<Shader>(shader));
    }

    recompile();
}

OpenGLShaderGroup::~OpenGLShaderGroup() {
    if (_id > 0) {
        glDeleteProgram(_id);
        _id = 0;
    }
}

void OpenGLShaderGroup::recompile() {
    if (_id > 0) {
        glDeleteProgram(_id);
        _id = 0;
    }

    // Recompile shaders
    for (auto& shader : _shaders)
        shader->recompile();

    _id = glCreateProgram();

    // Attach shaders
    for (auto shader : _shaders)
        glAttachShader(_id, std::static_pointer_cast<OpenGLShader>(shader)->getId());

    // Link shaders
    glLinkProgram(_id);

    int success;
    char infoLog[512];
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        std::vector<std::string> shaderPaths;
        for (auto shader : _shaders)
            shaderPaths.push_back(fs::absolute(shader->getFilepath()).string());
        ASSERT(false, "Failed to link shader group [w]$0[]:\n[*w]Shaders:[w]$1\n[*w]Error:[w]\n$2", _debugName, shaderPaths, infoLog);
    }

    // Detach and delete shaders
    for (auto shader : _shaders) {
        std::shared_ptr<OpenGLShader> s = std::static_pointer_cast<OpenGLShader>(shader);
        glDetachShader(_id, s->getId());
    }
}

void OpenGLShaderGroup::bind() { glUseProgram(_id); }

void OpenGLShaderGroup::setBool(const char* name, bool b) { glUniform1ui(getLoc(name), (unsigned int)b); }

void OpenGLShaderGroup::setInt(const char* name, int i) { glUniform1i(getLoc(name), i); }

void OpenGLShaderGroup::setFloat(const char* name, float f) { glUniform1f(getLoc(name), f); }

void OpenGLShaderGroup::setVec2(const char* name, vec2 v) { glUniform2fv(getLoc(name), 1, &v.x); }

void OpenGLShaderGroup::setVec3(const char* name, vec3 v) { glUniform3fv(getLoc(name), 1, &v.x); }

void OpenGLShaderGroup::setVec4(const char* name, vec4 v) { glUniform4fv(getLoc(name), 1, &v.x); }

void OpenGLShaderGroup::setMat3(const char* name, mat3 m) { glUniformMatrix3fv(getLoc(name), 1, GL_FALSE, m.data); }

void OpenGLShaderGroup::setMat4(const char* name, mat4 m) { glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, m.data); }

void OpenGLShaderGroup::setImage(const char* name, StringId sid) {
    std::shared_ptr<OpenGLAPI> renderer = std::static_pointer_cast<OpenGLAPI>(graphics::getGraphicsAPI());
    std::shared_ptr<OpenGLImage> image = renderer->getOpenGLImages()[sid.getId()];
    static std::map<StringId, bool> lastWarns; // Used to avoid spamming warn

    if (!image) {
        if (!lastWarns[sid])
            LOG_WARN("graphics::OpenGLShaderGroup", "(setImage) Trying to use image that was never loaded: $0 = \"$1\"", name, sid);
        lastWarns[sid] = true;
        return;
    }
    lastWarns[sid] = false;

    int imgUnit = -1;
    for (unsigned i = 0; i < _textureUnits.size(); i++)
        if (_textureUnits[i] == name) {
            imgUnit = int(i) + 1;
            break;
        }

    if (imgUnit == -1) {
        LOG_WARN("graphics::OpenGLShaderGroup", "(setImage) Trying to set texture [w]$0[], that was not found in the fragment shader code", name);
        return;
    }

    // Ensure image is set to write texture unit
    GLint imageLocation = glGetUniformLocation(_id, name);
    glUniform1i(imageLocation, imgUnit);

    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + imgUnit);
    glBindTexture(GL_TEXTURE_2D, image->getId());
}

void OpenGLShaderGroup::setImage(const char* name, std::shared_ptr<Image> inImage) {
    std::shared_ptr<OpenGLImage> image = std::static_pointer_cast<OpenGLImage>(inImage);

    if (!image) {
        LOG_WARN("graphics::OpenGLShaderGroup", "(setImage) Trying to set [w]$0[] with image that was never created", name);
        return;
    }

    int imgUnit = -1;
    for (unsigned i = 0; i < _textureUnits.size(); i++)
        if (_textureUnits[i] == name) {
            imgUnit = int(i) + 1;
            break;
        }

    if (imgUnit == -1) {
        LOG_WARN("graphics::OpenGLShaderGroup", "(setImage) Trying to set texture [w]$0[], that was not found in the fragment shader code", name);
        return;
    }

    // Ensure image is set to write texture unit
    GLint imageLocation = glGetUniformLocation(_id, name);
    glUniform1i(imageLocation, imgUnit);

    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + imgUnit);
    glBindTexture(GL_TEXTURE_2D, image->getId());
}

void OpenGLShaderGroup::setCubemap(const char* name, StringId sid) {
    std::shared_ptr<OpenGLAPI> renderer = std::static_pointer_cast<OpenGLAPI>(graphics::getGraphicsAPI());
    if (renderer->getOpenGLCubemaps().find(sid.getId()) == renderer->getOpenGLCubemaps().end()) {
        LOG_WARN("graphics::OpenGLShaderGroup", "(setCubemap) Trying to use cubemap that was never generated: $0 = \"$1\"", name, sid);
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
        LOG_WARN("graphics::OpenGLShaderGroup", "(setCubemap) Trying to set cubemap [w]$0[], that was not found in the fragment shader code", name);
        return;
    }

    // Ensure image is set to write texture unit
    GLint imageLocation = glGetUniformLocation(_id, name);
    glUniform1i(imageLocation, imgUnit);

    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + imgUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    // LOG_DEBUG("graphics::OpenGLShaderGroup", "Bind $0 to texture unit $1, id $2 -> $3", name, imgUnit, cubemap, _textureUnits);
}

void OpenGLShaderGroup::setCubemap(const char* name, std::shared_ptr<Image> inImage) {
    std::shared_ptr<OpenGLImage> image = std::static_pointer_cast<OpenGLImage>(inImage);

    if (!image) {
        LOG_WARN("graphics::OpenGLShaderGroup", "(setCubemap) Trying to set [w]$0[] with image that was never created", name);
        return;
    }

    if (!image->isCubemap()) {
        LOG_WARN("graphics::OpenGLShaderGroup", "(setCubemap) Trying to set [w]$0[] with image that is not a cubemap", name);
        return;
    }

    int imgUnit = -1;
    for (unsigned i = 0; i < _textureUnits.size(); i++)
        if (_textureUnits[i] == name) {
            imgUnit = int(i) + 1;
            break;
        }

    if (imgUnit == -1) {
        LOG_WARN("graphics::OpenGLShaderGroup", "(setCubemap) Trying to set cubemap [w]$0[], that was not found in the fragment shader code", name);
        return;
    }

    // Ensure image is set to write texture unit
    GLint imageLocation = glGetUniformLocation(_id, name);
    glUniform1i(imageLocation, imgUnit);

    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + imgUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, image->getId());
}

unsigned int OpenGLShaderGroup::getLoc(const char* name) { return glGetUniformLocation(_id, name); }

} // namespace atta::graphics
