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
#include <atta/graphics/apis/vulkan/uniformBuffer.h>
#include <atta/graphics/shader.h>

namespace atta::graphics::vk {

class Shader final : public gfx::Shader {
  public:
    Shader(const fs::path& file);
    ~Shader();

    std::string generateApiCode(ShaderType type, std::string iCode) override;
    void compile() override;
    void bind() override;
    void unbind() override;

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
    std::shared_ptr<UniformBuffer> getUniformBuffer() const;
    std::vector<std::shared_ptr<gfx::Image>> getUniformImages() const;
    void pushUniformBuffer();

  private:
    static VkShaderStageFlagBits convertFileToShaderStage(const fs::path& filepath);

    void updateUniformBuffer(const char* name, uint8_t* data, size_t size);
    void updateImage(const char* name, std::shared_ptr<gfx::Image> image);

    bool runCommand(std::string cmd);
    std::string readFile(const fs::path& file);

    std::shared_ptr<Device> _device;
    std::map<ShaderType, VkShaderModule> _shaders;
    std::vector<uint8_t> _uniformBufferData;
    std::shared_ptr<UniformBuffer> _uniformBuffer;
    std::vector<std::shared_ptr<gfx::Image>> _uniformImages;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SHADER_MODULE_H
