//--------------------------------------------------
// Atta Graphics Module
// descriptorSetLayout.h
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_SET_LAYOUT_H
#define ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_SET_LAYOUT_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class DescriptorSetLayout {
  public:
    struct Binding {
        uint32_t binding;         ///< Layout index in the shader
        uint32_t descriptorCount; ///< Number of descriptors to bind
        VkDescriptorType type;    ///< Type of the bound descriptor(s)
        VkShaderStageFlags stage; ///< Shader stage at which the bound resources will be available
    };
    DescriptorSetLayout(std::vector<Binding> bindings);
    ~DescriptorSetLayout();

    VkDescriptorSetLayout getHandle() const;
    std::shared_ptr<Device> getDevice() const;
    std::vector<Binding> getBindings() const;

  private:
    VkDescriptorSetLayout _descriptorSetLayout;
    std::shared_ptr<Device> _device;
    std::vector<Binding> _bindings;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_SET_LAYOUT_H
