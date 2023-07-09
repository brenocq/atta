//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/manager.h>
#include <atta/graphics/shader.h>

namespace atta::graphics {

Shader::Shader(const fs::path& file) : _file(file) {
    // Check file
    if (!fs::exists(file)) {
        LOG_ERROR("gfx::Shader", "Shader file [w]$0[] was not found", fs::absolute(file).string());
        return;
    }

    if (file.extension() != ".asl") {
        LOG_ERROR("gfx::Shader", "Shader file [w]$0[] does not have [w].asl[] extension", file.string());
        return;
    }

    // Read file
    std::stringstream ss;
    fs::path absolutePath = file::solveResourcePath(_file);
    std::ifstream f(absolutePath);
    ss << f.rdbuf();
    f.close();
    _aslCode = ss.str();

    preprocessASL();
}

fs::path Shader::getFile() const { return _file; }

VertexBufferLayout Shader::getVertexBufferLayout() const { return _vertexBufferLayout; }

void Shader::preprocessASL() { LOG_DEBUG("gfx::Shader", "Preprocess ASL: [w]$0", _file.string()); }

} // namespace atta::graphics
