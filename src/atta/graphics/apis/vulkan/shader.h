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

    void compile() override;
    void bind() override;

    void setBool(const char* name, bool b) override;
    void setInt(const char* name, int i) override;
    void setFloat(const char* name, float f) override;
    void setVec2(const char* name, vec2 v) override;
    void setVec3(const char* name, vec3 v) override;
    void setVec4(const char* name, vec4 v) override;
    void setMat3(const char* name, mat3 m) override;
    void setMat4(const char* name, mat4 m) override;
    void setImage(const char* name, StringId sid) override;
    void setImage(const char* name, std::shared_ptr<gfx::Image> image) override;
    void setCubemap(const char* name, StringId sid) override;
    void setCubemap(const char* name, std::shared_ptr<gfx::Image> image) override;

    std::vector<VkPipelineShaderStageCreateInfo> getShaderStages() const;

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
