// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/buffer.h>
#include <atta/graphics/vertexBuffer.h>

namespace atta::graphics::vk {

class VertexBuffer final : public Buffer, public gfx::VertexBuffer {
  public:
    VertexBuffer(const gfx::VertexBuffer::CreateInfo& info);
    ~VertexBuffer();

    void bind() const override;
    void bind(VkCommandBuffer commandBuffer) const;

    void update(const uint8_t* data, uint32_t size) override;

    static VkVertexInputBindingDescription getBindingDescription(const BufferLayout& layout);
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(const BufferLayout& layout);

  private:
    static VkFormat convertType(BufferLayout::Element::Type type);
};

} // namespace atta::graphics::vk
