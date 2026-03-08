// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class WindowResize : public EventTyped<SID("WindowResize")> {
  public:
    WindowResize(size_t width_, size_t height_) : width(width_), height(height_) {}

    size_t width, height;
};

} // namespace atta::event
