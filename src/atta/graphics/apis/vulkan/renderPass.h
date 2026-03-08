// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>
#include <atta/graphics/renderPass.h>

namespace atta::graphics::vk {

class Framebuffer;
class RenderPass final : public gfx::RenderPass {
  public:
    RenderPass(const gfx::RenderPass::CreateInfo& info);
    ~RenderPass();

    void begin(std::shared_ptr<RenderQueue> renderQueue) override;
    void end() override;

    void begin(VkCommandBuffer commandBuffer);
    void end(VkCommandBuffer commandBuffer);

    VkRenderPass getHandle() const;

    void setFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

  private:
    VkRenderPass _renderPass;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk
