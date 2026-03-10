// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::graphics {

class RenderQueue {
  public:
    RenderQueue();
    virtual ~RenderQueue() = default;

    virtual void begin() = 0;
    virtual void end() = 0;
};

} // namespace atta::graphics
