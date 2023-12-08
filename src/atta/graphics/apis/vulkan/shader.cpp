//--------------------------------------------------
// Atta Graphics Module
// shader.cpp
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/shader.h>

#include <atta/file/manager.h>
#include <regex>

namespace atta::graphics::vk {

Shader::Shader(const fs::path& file) : gfx::Shader(file), _device(common::getDevice()) {
    for (auto& [type, shaderCode] : _shaderCodes)
        shaderCode.apiCode = generateApiCode(type, shaderCode.iCode);
    compile();
}

Shader::~Shader() {
    for (auto& [type, shader] : _shaders)
        if (shader != VK_NULL_HANDLE)
            vkDestroyShaderModule(_device->getHandle(), shader, nullptr);
}

void Shader::setBool(const char* name, bool b) {}
void Shader::setInt(const char* name, int i) {}
void Shader::setFloat(const char* name, float f) {}
void Shader::setVec2(const char* name, vec2 v) {}
void Shader::setVec3(const char* name, vec3 v) {}
void Shader::setVec4(const char* name, vec4 v) {}
void Shader::setMat3(const char* name, mat3 m) {}
void Shader::setMat4(const char* name, mat4 m) {}
void Shader::setImage(const char* name, StringId sid) {}
void Shader::setImage(const char* name, std::shared_ptr<gfx::Image> image) {}
void Shader::setCubemap(const char* name, StringId sid) {}
void Shader::setCubemap(const char* name, std::shared_ptr<gfx::Image> image) {}

std::vector<VkPipelineShaderStageCreateInfo> Shader::getShaderStages() const {
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    for (auto& [type, shader] : _shaders) {
        VkPipelineShaderStageCreateInfo shaderStageInfo;
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.module = shader;
        shaderStageInfo.pName = "main";
        switch (type) {
            case VERTEX:
                shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                break;
            case GEOMETRY:
                shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
                break;
            case FRAGMENT:
                shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                break;
        }
        shaderStages.push_back(shaderStageInfo);
    }

    return shaderStages;
}

std::string Shader::generateApiCode(ShaderType type, std::string iCode) {
    std::string apiCode;

    // GLSL version
    apiCode += "#version 450\n";

    // Uniform buffer
    if (!_uniformLayout.getElements().empty()) {
        apiCode += "layout(set = 0, binding = 0) uniform UniformBufferObject {\n";
        for (const BufferLayout::Element& element : _uniformLayout.getElements())
            if (element.type != BufferLayout::Element::Type::SAMPLER_2D && element.type != BufferLayout::Element::Type::SAMPLER_CUBE)
                apiCode += std::string("    ") + BufferLayout::Element::typeToString(element.type) + " " + element.name + ";\n";
        apiCode += "} ubo;\n";
    }

    // TODO Uniform image sampler

    // Remove uniform declarations from iCode
    std::regex pattern(R"(^uniform .*\n?)", std::regex_constants::multiline);
    std::string iCodeFix = std::regex_replace(iCode, pattern, "");

    // Append "ubo." when using uniform in iCode
    for (const BufferLayout::Element& element : _uniformLayout.getElements()) {
        if (element.type != BufferLayout::Element::Type::SAMPLER_2D && element.type != BufferLayout::Element::Type::SAMPLER_CUBE) {
            std::string patternStr = "\\b" + element.name + "\\b";
            std::regex pattern(patternStr);
            iCodeFix = std::regex_replace(iCodeFix, pattern, "ubo." + element.name);
        }
    }

    // Add corrected iCode to apiCode
    apiCode += iCodeFix;

    // Add location to in/out
    std::istringstream iss(apiCode);
    std::ostringstream oss;
    std::string line;
    int inNum = 0, outNum = 0;
    while (std::getline(iss, line)) {
        if (std::regex_search(line, std::regex(R"(^in\s)"))) {
            oss << "layout(location = " << inNum++ << ") " << line << "\n";
        } else if (std::regex_search(line, std::regex(R"(^out\s)"))) {
            oss << "layout(location = " << outNum++ << ") " << line << "\n";
        } else {
            oss << line << "\n";
        }
    }
    apiCode = oss.str();

    // Add location to out

    // LOG_DEBUG("gfx::vk::Shader", "[y]$0\n[r]iCode:\n[w]$1\n[r]apiCode:\n[w]$2", type, iCode, apiCode);

    return apiCode;
}

void Shader::compile() {
    for (const auto& [type, shaderCode] : _shaderCodes) {
        // Save shader
        std::array<std::string, 3> ext = {".vert", ".geom", ".frag"};
        fs::path in = file::getBuildPath() / "shaders" / (_file.stem().string() + ext[int(type)]);
        fs::create_directories(in.parent_path());
        std::ofstream file(in);
        if (file.is_open()) {
            file << shaderCode.apiCode;
            file.close();
            LOG_DEBUG("shader", "Saved $0 code", in);
        } else {
            LOG_ERROR("gfx::vk::Shader", "Failed to save shader code to [w]$0[] when compiling [w]$1[]", in, _file.string());
            continue;
        }

        // Compile shader
        fs::path out = in.string() + ".spv";
        LOG_DEBUG("Shader", "in $0 out $1", in, out);
        if (!runCommand("glslc " + in.string() + " -o " + out.string()))
            return;

        // Load shader and create shader module
        std::string spirvCode = readFile(fs::path(out));
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = spirvCode.size();
        createInfo.pCode = (const uint32_t*)spirvCode.data();

        VkShaderModule shader;
        if (vkCreateShaderModule(_device->getHandle(), &createInfo, nullptr, &shader) != VK_SUCCESS)
            LOG_ERROR("gfx::vk::Shader", "Failed to create shader!");
        else
            _shaders[type] = shader;
    }
}

void Shader::bind() {}

VkShaderModule Shader::getHandle() const {
    // TODO
    return {};
}

VkPipelineShaderStageCreateInfo Shader::getShaderStage() const {
    // VkShaderStageFlagBits stage = convertFileToShaderStage(_filepath);

    VkPipelineShaderStageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // createInfo.stage = stage;
    // createInfo.module = _shader;
    createInfo.pName = "main";

    return createInfo;
}

VkShaderStageFlagBits Shader::convertFileToShaderStage(const fs::path& filepath) {
    // std::string extension = filepath.extension().string();
    // if (extension == ".vert")
    //     return VK_SHADER_STAGE_VERTEX_BIT;
    // if (extension == ".frag")
    //     return VK_SHADER_STAGE_FRAGMENT_BIT;
    // if (extension == ".geom")
    //     return VK_SHADER_STAGE_GEOMETRY_BIT;
    // ASSERT(false, "Unknown shader file format [w]$0[]. Instead of [*w]$1[], it should be [w].vert[], [w].frag[], or [w].geom[]", filepath.string(),
    //        extension);
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
        LOG_ERROR("gfx::vk::Shader", "Failed to compile shader [w]$0[]:\n$1", _file.string(), output);
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

} // namespace atta::graphics::vk
