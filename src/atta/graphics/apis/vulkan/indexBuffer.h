// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/buffer.h>
#include <atta/graphics/indexBuffer.h>

namespace atta::graphics::vk {

class IndexBuffer final : public Buffer, public gfx::IndexBuffer {
  public:
    IndexBuffer(const gfx::IndexBuffer::CreateInfo& info);
    ~IndexBuffer();

    void bind() const override;
    void bind(VkCommandBuffer commandBuffer) const;
};

} // namespace atta::graphics::vk
