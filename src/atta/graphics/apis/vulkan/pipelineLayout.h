//--------------------------------------------------
// Atta Graphics Module
// pipelineLayout.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_PIPELINE_LAYOUT_H
#define ATTA_GRAPHICS_APIS_VULKAN_PIPELINE_LAYOUT_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/descriptorSetLayout.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/pushConstant.h>

namespace atta::graphics::vk {

class PipelineLayout {
  public:
    PipelineLayout(std::shared_ptr<DescriptorSetLayout> descriptorSetLayout, std::shared_ptr<PushConstant> pushConstant);
    ~PipelineLayout();

    VkPipelineLayout getHandle() const;

  private:
    VkPipelineLayout _pipelineLayout;
    std::shared_ptr<Device> _device;
    std::shared_ptr<DescriptorSetLayout> _descriptorSetLayout;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_PIPELINE_LAYOUT_H
