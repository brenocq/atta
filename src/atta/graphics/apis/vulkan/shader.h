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

namespace atta::graphics::vk {

class Shader {
  public:
    Shader(std::shared_ptr<Device> device, const std::string& filename);
    ~Shader();

    VkShaderModule getHandle() const;

    VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits stage) const;

  private:
    std::vector<char> readFile(const std::string& filename);

    std::shared_ptr<Device> _device;
    VkShaderModule _shader;
    std::vector<char> _code;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_SHADER_MODULE_H
