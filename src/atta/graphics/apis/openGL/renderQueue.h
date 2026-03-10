// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/renderQueue.h>

namespace atta::graphics::gl {

class RenderQueue final : public gfx::RenderQueue {
  public:
    void begin() override;
    void end() override;
};

} // namespace atta::graphics::gl
