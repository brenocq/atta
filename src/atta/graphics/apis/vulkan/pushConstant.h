// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/common.h>

namespace atta::graphics::vk {

class PushConstant final {
  public:
    PushConstant(size_t size);

    void write(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const std::vector<uint8_t>& data);

    const VkPushConstantRange& getRange() const;
    size_t getSize() const;

  private:
    VkPushConstantRange _range;
    size_t _size; ///< Size of the push constant in bytes
};

} // namespace atta::graphics::vk
