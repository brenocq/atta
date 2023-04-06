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

class Shader final : public graphics::Shader {
  public:
    Shader(const graphics::Shader::CreateInfo& info);
    ~Shader();

    void recompile() override;

    VkShaderModule getHandle() const;
    VkPipelineShaderStageCreateInfo getShaderStage() const;

  private:
    static VkShaderStageFlagBits convertFileToShaderStage(const fs::path& filepath);
    std::vector<char> readFile(const std::string& filepath);

    std::shared_ptr<Device> _device;
    VkShaderModule _shader;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SHADER_MODULE_H
