//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/shader.h>

#include <atta/file/manager.h>

namespace atta::graphics::vk {

Shader::Shader(const gfx::Shader::CreateInfo& info) : gfx::Shader(info), _device(common::getDevice()) { recompile(); }

Shader::~Shader() {
    if (_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(_device->getHandle(), _shader, nullptr);
}

void Shader::recompile() {
    fs::path filepath = file::solveResourcePath(_filepath);
    std::string in = fs::absolute(filepath).string();
    std::string out = in + ".spv";
    LOG_DEBUG("sf", "$0 $1", in, out);
    if(!runCommand("glslc " + in + " -o " + out))
        return;

    // Parse GLSL
    parseGlsl(readFile(fs::path(in)));

    // Create vulkan
    std::string spirvCode = readFile(fs::path(out));
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = spirvCode.size();
    createInfo.pCode = (const uint32_t*)spirvCode.data();

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

const Shader::ParsedData& Shader::getParsedData() const { return _parsedData; }

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

bool Shader::runCommand(std::string cmd) {
    std::array<char, 512> buffer;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((cmd + " 2>&1").c_str(), "r"), pclose);
    if (!pipe) {
        LOG_ERROR("gfx::vk::Shader", "Could not open pipe");
        return false;
    }

    // Get command output
    std::string output;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        output += buffer.data();

    if (output != "") {
        LOG_ERROR("gfx::vk::Shader", "Failed to compile shader [w]$0[]: $1", _filepath.string(), output);
        return false;
    }
    return true;
}

std::string Shader::readFile(const fs::path& file) {
    std::ifstream f(file, std::ios::ate | std::ios::binary);

    if (!f.is_open()) {
        LOG_ERROR("gfx::vk::Shader", "Failed to open file: [w]$0[]!", file.string());
        return {};
    }

    size_t fileSize = (size_t)f.tellg();
    std::vector<char> buffer(fileSize);
    f.seekg(0);
    f.read(buffer.data(), fileSize);
    f.close();

    return std::string(buffer.begin(), buffer.end());
}

void Shader::parseGlsl(std::string code) {

}

} // namespace atta::graphics::vk
