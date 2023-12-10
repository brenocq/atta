//--------------------------------------------------
// Atta Graphics Module
// pipeline.h
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_PIPELINE_H
#define ATTA_GRAPHICS_APIS_VULKAN_PIPELINE_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/descriptorPool.h>
#include <atta/graphics/apis/vulkan/descriptorSetLayout.h>
#include <atta/graphics/apis/vulkan/descriptorSets.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/apis/vulkan/framebuffer.h>
#include <atta/graphics/apis/vulkan/image.h>
#include <atta/graphics/apis/vulkan/pipelineLayout.h>
#include <atta/graphics/apis/vulkan/renderPass.h>
#include <atta/graphics/apis/vulkan/vertexBuffer.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics::vk {

class Pipeline final : public gfx::Pipeline {
  public:
    Pipeline(const gfx::Pipeline::CreateInfo& info);
    ~Pipeline();

    void begin(std::shared_ptr<RenderQueue> renderQueue) override;
    void end() override;
    void* getImGuiTexture() const override;

    VkPipeline getHandle() const;
    std::shared_ptr<PipelineLayout> getPipelineLayout() const;

  private:
    VkPipeline _pipeline;
    std::shared_ptr<Device> _device;
    std::vector<std::shared_ptr<Framebuffer>> _framebuffers;

    std::shared_ptr<PipelineLayout> _pipelineLayout;
    std::shared_ptr<DescriptorPool> _descriptorPool;
    std::shared_ptr<DescriptorSetLayout> _descriptorSetLayout;
    std::shared_ptr<DescriptorSets> _descriptorSets;

    // Output image info
    VkExtent2D _imageExtent;
    VkFormat _imageFormat;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_PIPELINE_H
