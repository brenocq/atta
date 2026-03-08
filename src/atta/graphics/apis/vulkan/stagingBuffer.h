// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/buffer.h>

namespace atta::graphics::vk {

class StagingBuffer final : public Buffer {
  public:
    StagingBuffer(const uint8_t* data, size_t size);
    ~StagingBuffer();
};

} // namespace atta::graphics::vk
