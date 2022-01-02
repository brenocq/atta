//--------------------------------------------------
// Atta Graphics System
// openGLShader.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_SHADER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_SHADER_H
#include <atta/graphicsSystem/shader.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>

namespace atta
{
    class OpenGLShader final : public Shader
    {
    public:
        OpenGLShader(const Shader::CreateInfo& info);
        ~OpenGLShader();

        void recompile() override;

        OpenGLId getId() const { return _id; }
        std::vector<std::string> getTextureUnits() { return _textureUnits; }
        void deleteShader();

        static GLenum convertFileToShaderType(const fs::path& filepath);

    private:
        void extractTextureUnits(const std::stringstream& sstream);

        OpenGLId _id;
        std::vector<std::string> _textureUnits;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_SHADER_H
