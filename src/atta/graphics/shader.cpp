//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/shader.h>

namespace atta::graphics {

Shader::Shader(const CreateInfo& info) : _filepath(info.filepath) {}

} // namespace atta::graphics
