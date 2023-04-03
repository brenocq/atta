//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/shader.h>

#include <atta/file/manager.h>

namespace atta::graphics::vk {

Shader::Shader(std::shared_ptr<Device> device, const std::string& filename) : _device(device) {
    _code = readFile(filename);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = _code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(_code.data());

    if (vkCreateShaderModule(_device->getHandle(), &createInfo, nullptr, &_shader) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Shader", "Failed to create shader!");
}

Shader::~Shader() { vkDestroyShaderModule(_device->getHandle(), _shader, nullptr); }

VkShaderModule Shader::getHandle() const { return _shader; }

std::vector<char> Shader::readFile(const std::string& filename) {
    std::ifstream file(file::solveResourcePath(filename), std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        LOG_ERROR("Shader", "Failed to open file: [w]$0[]!", filename);
        return {};
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkPipelineShaderStageCreateInfo Shader::createShaderStage(VkShaderStageFlagBits stage) const {
    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    createInfo.stage = stage;
    createInfo.module = _shader;
    createInfo.pName = "main";

    return createInfo;
}

} // namespace atta::graphics::vk
