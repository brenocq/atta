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

    // Create Uniform buffer (TODO only supports 1000 uniform buffers)
    _uniformBuffer = std::make_shared<UniformBuffer>(_uniformLayout.getStride(), 1000);
}

Shader::~Shader() {
    for (auto& [type, shader] : _shaders)
        if (shader != VK_NULL_HANDLE)
            vkDestroyShaderModule(_device->getHandle(), shader, nullptr);
}

void Shader::setBool(const char* name, bool b) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&b), sizeof(bool)); }
void Shader::setInt(const char* name, int i) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&i), sizeof(int)); }
void Shader::setFloat(const char* name, float f) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&f), sizeof(float)); }
void Shader::setVec2(const char* name, vec2 v) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&v), sizeof(vec2)); }
void Shader::setVec3(const char* name, vec3 v) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&v), sizeof(vec3)); }
void Shader::setVec4(const char* name, vec4 v) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&v), sizeof(vec4)); }
void Shader::setMat3(const char* name, mat3 m) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&m), sizeof(mat3)); }
void Shader::setMat4(const char* name, mat4 m) { updateUniformBuffer(name, reinterpret_cast<uint8_t*>(&m), sizeof(mat4)); }
void Shader::setImage(const char* name, StringId sid) {
    if (Manager::getInstance().getImages().find(sid) == Manager::getInstance().getImages().end()) {
        LOG_WARN("gfx::vk::Shader", "Could not set image [w]$0[] to uniform image [w]$1[], image not found", sid, name);
        return;
    }
    updateImage(name, Manager::getInstance().getImages().at(sid));
}
void Shader::setImage(const char* name, std::shared_ptr<gfx::Image> image) { updateImage(name, image); }
void Shader::setCubemap(const char* name, StringId sid) {
    if (Manager::getInstance().getImages().find(sid) == Manager::getInstance().getImages().end()) {
        LOG_WARN("gfx::vk::Shader", "Could not set cubemap [w]$0[] to uniform image [w]$1[], image not found", sid, name);
        return;
    }
    updateImage(name, Manager::getInstance().getImages().at(sid));
}

void Shader::setCubemap(const char* name, std::shared_ptr<gfx::Image> image) { updateImage(name, image); }

void Shader::updateUniformBuffer(const char* name, uint8_t* data, size_t size) {
    if (_uniformBufferData.empty()) {
        LOG_ERROR("gfx::vk::Shader",
                  "Shader uniform [w]$0[] should only be set while rendering. Make sure it was called while rendering the pipeline.", name);
        return;
    }
    if (!_uniformLayout.exists(name)) {
        LOG_WARN("gfx::vk::Shader", "Trying to update [w]$0[], but this uniform was not declared in the shader", name);
        return;
    }

    // Find element to update
    for (const BufferLayout::Element& element : _uniformLayout.getElements()) {
        if (element.name == name) {
            // Update uniform buffer
            for (size_t i = 0; i < size; i++)
                _uniformBufferData[element.offset + i] = data[i];
            return;
        }
    }
}

void Shader::updateImage(const char* name, std::shared_ptr<gfx::Image> image) {
    if (!_imageLayout.exists(name)) {
        LOG_WARN("gfx::vk::Shader", "Trying to update [w]$0[], but this uniform image was not declared in the shader", name);
        return;
    }

    // Find element to update
    size_t idx = 0;
    for (const BufferLayout::Element& element : _imageLayout.getElements()) {
        if (element.name == name) {
            _uniformImages[idx] = image;
            return;
        }
        idx++;
    }
}

std::vector<VkPipelineShaderStageCreateInfo> Shader::getShaderStages() const {
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    for (auto& [type, shader] : _shaders) {
        VkPipelineShaderStageCreateInfo shaderStageInfo{};
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

std::shared_ptr<UniformBuffer> Shader::getUniformBuffer() const { return _uniformBuffer; }

std::vector<std::shared_ptr<gfx::Image>> Shader::getUniformImages() const { return _uniformImages; }

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

    // Uniform image sampler
    size_t bindingIdx = 1;
    for (const BufferLayout::Element& element : _imageLayout.getElements()) {
        apiCode += "layout(binding = " + std::to_string(bindingIdx) + ") uniform " + BufferLayout::Element::typeToString(element.type) + " " +
                   element.name + ";\n";
        bindingIdx++;
    }

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
        } else {
            LOG_ERROR("gfx::vk::Shader", "Failed to save shader code to [w]$0[] when compiling [w]$1[]", in, _file.string());
            continue;
        }

        // Compile shader
        fs::path out = in.string() + ".spv";
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

void Shader::bind() {
    _uniformBufferIdx = 0;
    _uniformBufferData.resize(_uniformLayout.getStride());
    _uniformImages.resize(_uniformLayout.getElementCount());
    // TODO point uniformImages to purple image by default
}

void Shader::unbind() {
    _uniformBufferData.clear();
    _uniformImages.clear();
}

uint32_t Shader::pushUniformBuffer() {
    if (_uniformBufferIdx < _uniformBuffer->getNumInstances()) {
        size_t idx = _uniformBufferIdx;
        _uniformBufferIdx++;
        _uniformBuffer->writeInstance(idx, _uniformBufferData);
        return _uniformBuffer->getInstanceOffset(idx);
    } else
        LOG_WARN("gfx::vk::Shader", "It was not possible to write uniform buffer object, maximum number of instances was reached");
    return _uniformBufferIdx;
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
