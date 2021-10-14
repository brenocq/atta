//--------------------------------------------------
// Atta Graphics System
// openGLShader.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShader.h>
#include <atta/fileSystem/fileManager.h>

namespace atta
{
    OpenGLShader::OpenGLShader(const Shader::CreateInfo& info):
        Shader(info), _id(0)
    {
        recompile();
    }

    OpenGLShader::~OpenGLShader()
    {
        if(!_id)
            deleteShader();
    }

    void OpenGLShader::recompile()
    {
        if(!_id)
            deleteShader();

        // Create shader
        _id = glCreateShader(convertFileToShaderType(_filepath));

        // Read file
        std::stringstream fileSS;
        fs::path absolutePath = FileManager::solveResourcePath(_filepath);
        std::ifstream file(absolutePath);
        fileSS << file.rdbuf();
        file.close();

        // Compile
        std::string codeStr = fileSS.str();
        const char* code = codeStr.c_str();
        glShaderSource(_id, 1, &code, NULL);
        glCompileShader(_id);

        // Check errors
        int success;
        char infoLog[512];
        glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(_id, 512, NULL, infoLog);
            ASSERT(false, "Failed to compile shader [*w]$0[]:[w]\n$1", _filepath.string(), infoLog);
        }
    }

    void OpenGLShader::deleteShader()
    {
        glDeleteShader(_id);
        _id = 0;
    }

    //------------------------------------------------//
    //---------- Atta to OpenGL conversions ----------//
    //------------------------------------------------//
    GLenum OpenGLShader::convertFileToShaderType(const fs::path& filepath)
    {
        std::string extension = filepath.extension().string();
        if(extension == ".vert") return GL_VERTEX_SHADER;
        if(extension == ".frag") return GL_FRAGMENT_SHADER;
        ASSERT(false, "Unknown shader file format [w]$0[]. Instead of [*w]$1[], it should be [w].vert[] or [w].frag[]",
                filepath.string(), extension);
    }
}
