//--------------------------------------------------
// Atta Graphics Module
// shaderGroup.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/shaderGroup.h>

#include <atta/graphics/apis/vulkan/shader.h>

namespace atta::graphics::vk {

ShaderGroup::ShaderGroup(const graphics::ShaderGroup::CreateInfo& info, std::shared_ptr<Device> device)
    : graphics::ShaderGroup(info), _device(device) {
    // Create shaders (they are compiled at creation)
    for (fs::path shaderPath : info.shaderPaths) {
        // Create shader
        std::shared_ptr<vk::Shader> shader = std::make_shared<vk::Shader>(shaderPath, device);
        // Save shader
        _shaders.push_back(std::static_pointer_cast<graphics::Shader>(shader));
    }

    recompile();
}

ShaderGroup::~ShaderGroup() {}

void ShaderGroup::recompile() {}

void ShaderGroup::bind() {}

void ShaderGroup::setBool(const char* name, bool b) {}
void ShaderGroup::setInt(const char* name, int i) {}
void ShaderGroup::setFloat(const char* name, float f) {}
void ShaderGroup::setVec2(const char* name, vec2 v) {}
void ShaderGroup::setVec3(const char* name, vec3 v) {}
void ShaderGroup::setVec4(const char* name, vec4 v) {}
void ShaderGroup::setMat3(const char* name, mat3 m) {}
void ShaderGroup::setMat4(const char* name, mat4 m) {}
void ShaderGroup::setImage(const char* name, StringId sid) {}
void ShaderGroup::setImage(const char* name, std::shared_ptr<gfx::Image> image) {}
void ShaderGroup::setCubemap(const char* name, StringId sid) {}
void ShaderGroup::setCubemap(const char* name, std::shared_ptr<gfx::Image> image) {}

std::vector<VkPipelineShaderStageCreateInfo> ShaderGroup::getShaderStages() const {
    std::vector<VkPipelineShaderStageCreateInfo> result;
    for (std::shared_ptr<graphics::Shader> s : _shaders)
        result.push_back(std::dynamic_pointer_cast<vk::Shader>(s)->getShaderStage());
    return result;
}

} // namespace atta::graphics::vk
