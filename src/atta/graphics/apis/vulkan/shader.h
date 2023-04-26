//--------------------------------------------------
// Atta Graphics Module
// shader.h
// Date: 2023-04-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_SHADER_MODULE_H
#define ATTA_GRAPHICS_APIS_VULKAN_SHADER_MODULE_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/shader.h>

namespace atta::graphics::vk {

class Shader final : public gfx::Shader {
  public:
    enum Type {
        INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
        SAMPLER_2D,
        SAMPLER_CUBE,
    };

    struct Element {
        std::string name; ///< Element name
        Type type;        ///< Element type in GLSL
        size_t offset;    ///< Byte offset in the uniform buffer
    };

    struct ParsedData {
        std::vector<uint8_t> uniformData;
        std::vector<Element> uniforms;
    };

    Shader(const gfx::Shader::CreateInfo& info);
    ~Shader();

    void recompile() override;

    VkShaderModule getHandle() const;
    VkPipelineShaderStageCreateInfo getShaderStage() const;
    const ParsedData& getParsedData() const;

  private:
    static VkShaderStageFlagBits convertFileToShaderStage(const fs::path& filepath);

    bool runCommand(std::string cmd);
    std::string readFile(const fs::path& file);
    void parseGlsl(std::string code);

    std::shared_ptr<Device> _device;
    VkShaderModule _shader;
    ParsedData _parsedData;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SHADER_MODULE_H
