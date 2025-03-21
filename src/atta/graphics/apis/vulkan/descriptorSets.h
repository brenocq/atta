//--------------------------------------------------
// Atta Graphics Module
// descriptorSets.h
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_SETS_H
#define ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_SETS_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/descriptorPool.h>
#include <atta/graphics/apis/vulkan/descriptorSetLayout.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/image.h>
#include <atta/graphics/apis/vulkan/pipelineLayout.h>
#include <atta/graphics/apis/vulkan/uniformBuffer.h>

namespace atta::graphics::vk {

class DescriptorSets {
  public:
    DescriptorSets(std::shared_ptr<DescriptorPool> descriptorPool, std::shared_ptr<DescriptorSetLayout> descriptorSetLayout,
                   std::shared_ptr<PipelineLayout> pipelineLayout, size_t size);

    ~DescriptorSets();

    std::vector<VkDescriptorSet> getHandle() const;
    std::shared_ptr<Device> getDevice() const;

    void update(uint32_t bindingIdx, std::shared_ptr<UniformBuffer> uniformBuffer) const;
    void update(uint32_t bindingIdx, std::shared_ptr<Image> image) const;
    void bind(VkCommandBuffer commandBuffer, uint32_t setIdx, size_t index = 0, const uint32_t* dynamicOffset = nullptr);

  private:
    std::shared_ptr<Device> _device;
    std::shared_ptr<DescriptorPool> _descriptorPool;
    std::shared_ptr<DescriptorSetLayout> _descriptorSetLayout;
    std::shared_ptr<PipelineLayout> _pipelineLayout;
    std::vector<VkDescriptorSet> _descriptorSets;
    std::vector<std::shared_ptr<UniformBuffer>> _uniformBuffers;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_DESCRIPTOR_SETS_H
