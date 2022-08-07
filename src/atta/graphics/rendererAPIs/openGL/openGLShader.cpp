//--------------------------------------------------
// Atta Graphics Module
// openGLShader.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/manager.h>
#include <atta/graphics/rendererAPIs/openGL/openGLShader.h>

namespace atta::graphics {

OpenGLShader::OpenGLShader(const Shader::CreateInfo& info) : Shader(info), _id(0) { recompile(); }

OpenGLShader::~OpenGLShader() {
    if (!_id)
        deleteShader();
}

void OpenGLShader::recompile() {
    if (!_id)
        deleteShader();

    // Create shader
    _id = glCreateShader(convertFileToShaderType(_filepath));

    // Read file
    std::stringstream fileSS;
    fs::path absolutePath = file::Manager::solveResourcePath(_filepath);
    std::ifstream file(absolutePath);
    fileSS << file.rdbuf();
    file.close();

    // Extract texture units from fragment shader
    if (convertFileToShaderType(_filepath) == GL_FRAGMENT_SHADER)
        extractTextureUnits(fileSS);

    // Compile
    std::string codeStr = fileSS.str();
    const char* code = codeStr.c_str();
    glShaderSource(_id, 1, &code, NULL);
    glCompileShader(_id);

    // Check errors
    int success;
    char infoLog[512];
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_id, 512, NULL, infoLog);
        ASSERT(false, "Failed to compile shader [*w]$0[]:[w]\n$1", _filepath.string(), infoLog);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void OpenGLShader::deleteShader() {
    glDeleteShader(_id);
    _id = 0;
}

void OpenGLShader::extractTextureUnits(const std::stringstream& sstream) {
    // TODO ignore commented sampler2D
    std::string s = sstream.str();

    int currIdx = 0;
    while (true) {
        // Check if line defined a sampler2D
        size_t marker2D = s.find("uniform sampler2D", currIdx);
        size_t markerCube = s.find("uniform samplerCube", currIdx);
        size_t marker = std::min(marker2D, markerCube);

        if (marker != std::string::npos) {
            // Extract uniform name
            size_t endMarker = s.find(' ', marker + 9) + 1;
            size_t endName = s.find(';', endMarker);
            if (endName == std::string::npos)
                continue;
            std::string name = s.substr(endMarker, endName - endMarker);

            // LOG_DEBUG("graphics::OpenGLShader", "Found texture unit: [w]$0[] ($1)", name, currIdx);
            _textureUnits.push_back(name);
            currIdx = endName + 1;
        } else
            break;
    }
}

//------------------------------------------------//
//---------- Atta to OpenGL conversions ----------//
//------------------------------------------------//
GLenum OpenGLShader::convertFileToShaderType(const fs::path& filepath) {
    std::string extension = filepath.extension().string();
    if (extension == ".vert")
        return GL_VERTEX_SHADER;
    if (extension == ".frag")
        return GL_FRAGMENT_SHADER;
    if (extension == ".geom")
        return GL_GEOMETRY_SHADER;
    ASSERT(false, "Unknown shader file format [w]$0[]. Instead of [*w]$1[], it should be [w].vert[], [w].frag[], or [w].geom[]", filepath.string(),
           extension);
    return 0;
}

} // namespace atta::graphics
