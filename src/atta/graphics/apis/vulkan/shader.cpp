//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/shader.h>

#include <atta/file/manager.h>

namespace atta::graphics::vk {

Shader::Shader(const std::string& filepath, std::shared_ptr<Device> device) : graphics::Shader({filepath}), _device(device) {}

Shader::~Shader() {
    if (_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(_device->getHandle(), _shader, nullptr);
}

void Shader::recompile() {
    // TODO compile hlsl to .spv
    std::vector<char> code = readFile(_filepath);
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(_device->getHandle(), &createInfo, nullptr, &_shader) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Shader", "Failed to create shader!");
}

VkShaderModule Shader::getHandle() const { return _shader; }

VkPipelineShaderStageCreateInfo Shader::getShaderStage() const {
    VkShaderStageFlagBits stage = convertFileToShaderStage(_filepath);

    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = stage;
    createInfo.module = _shader;
    createInfo.pName = "main";

    return createInfo;
}

std::vector<char> Shader::readFile(const std::string& filepath) {
    std::ifstream file(file::solveResourcePath(filepath), std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        LOG_ERROR("gfx::vk::Shader", "Failed to open file: [w]$0[]!", filepath);
        return {};
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderStageFlagBits Shader::convertFileToShaderStage(const fs::path& filepath) {
    std::string extension = filepath.extension().string();
    if (extension == ".vert")
        return VK_SHADER_STAGE_VERTEX_BIT;
    if (extension == ".frag")
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    if (extension == ".geom")
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    ASSERT(false, "Unknown shader file format [w]$0[]. Instead of [*w]$1[], it should be [w].vert[], [w].frag[], or [w].geom[]", filepath.string(),
           extension);
    return {};
}

} // namespace atta::graphics::vk
