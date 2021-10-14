//--------------------------------------------------
// Atta Graphics System
// openGLShaderGroup.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShaderGroup.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShader.h>

#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLRenderer.h>

namespace atta
{
    OpenGLShaderGroup::OpenGLShaderGroup(const ShaderGroup::CreateInfo& info):
        ShaderGroup(info), _id(0)
    {
        // Create shaders (they are compiled at creation)
        for(auto shaderPath : info.shaderPaths)
        {
            Shader::CreateInfo info {};
            info.filepath = shaderPath;
            _shaders.push_back(std::static_pointer_cast<Shader>(std::make_shared<OpenGLShader>(info)));
        }

        recompile();
    }

    OpenGLShaderGroup::~OpenGLShaderGroup()
    {
        if(_id > 0)
        {
            glDeleteProgram(_id);
            _id = 0;
        }
    }

    void OpenGLShaderGroup::recompile()
    {
        if(_id > 0)
        {
            glDeleteProgram(_id);
            _id = 0;
        }

        // Recompile shaders
        for(auto& shader : _shaders)
            shader->recompile();

        _id = glCreateProgram();

        // Attach shaders
        for(auto shader : _shaders)
            glAttachShader(_id, std::static_pointer_cast<OpenGLShader>(shader)->getId());

        // Link shaders
        glLinkProgram(_id);

        int success;
        char infoLog[512];
        glGetProgramiv(_id, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(_id, 512, NULL, infoLog);
            std::vector<std::string> shaderPaths;
            for(auto shader : _shaders)
                shaderPaths.push_back(fs::absolute(shader->getFilepath()).string());
            ASSERT(false, "Failed to link shader group [w]$0[]:\n[*w]Shaders:[w]$1\n[*w]Error:[w]\n$2", _debugName, shaderPaths, infoLog);
        }

        // Detach and delete shaders
        //glUseProgram(_id);
        for(auto shader : _shaders)
        {
            std::shared_ptr<OpenGLShader> s = std::static_pointer_cast<OpenGLShader>(shader);
            //glDetachShader(_id, s->getId());
            s->deleteShader();
        }
        //glUseProgram(0);
    }

    void OpenGLShaderGroup::bind()
    {
        glUseProgram(_id);
    }

    void OpenGLShaderGroup::setBool(const char* name, bool b)
    {
        glUniform1ui(getLoc(name), (unsigned int)b);
    }

    void OpenGLShaderGroup::setInt(const char* name, int i)
    {
        glUniform1i(getLoc(name), i);
    }

    void OpenGLShaderGroup::setFloat(const char* name, float f)
    {
        glUniform1f(getLoc(name), f);
    }

    void OpenGLShaderGroup::setVec2(const char* name, const vec2& v)
    {
        glUniform2fv(getLoc(name), 1, &v.x);
    }

    void OpenGLShaderGroup::setVec3(const char* name, const vec3& v)
    {
        glUniform3fv(getLoc(name), 1, &v.x);
    }

    void OpenGLShaderGroup::setVec4(const char* name, const vec4& v)
    {
        glUniform4fv(getLoc(name), 1, &v.x);
    }

    void OpenGLShaderGroup::setMat3(const char* name, const mat3& m)
    {
        glUniformMatrix3fv(getLoc(name), 1, GL_FALSE, m.data);
    }

    void OpenGLShaderGroup::setMat4(const char* name, const mat4& m)
    {
        glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, m.data);
    }

    void OpenGLShaderGroup::setTexture(const char* name, StringId sid)
    {
        std::shared_ptr<OpenGLRenderer> renderer = std::static_pointer_cast<OpenGLRenderer>(GraphicsManager::getRendererAPI());
        std::shared_ptr<OpenGLImage> image = renderer->getOpenGLImages()[sid.getId()];

        if(!image)
        {
            LOG_WARN("OpenGLShaderGroup", "(setTexture) Trying to use image that was never loaded $0", name);
            return;
        }

        GLint imgUnit = -1;
        glGetUniformiv(_id, getLoc(name), &imgUnit);

        if(imgUnit == -1)
        {
            LOG_WARN("OpenGLShaderGroup", "(setTexture) Could not get texture unit for $0", name);
            return;
        }
        glActiveTexture(GL_TEXTURE0+imgUnit);
        glBindTexture(GL_TEXTURE_2D, image->getId());
    }

    unsigned int OpenGLShaderGroup::getLoc(const char* name)
    {
        return glGetUniformLocation(_id, name);
    }
}
