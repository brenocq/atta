// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class WindowMouseMove : public EventTyped<SID("WindowMouseMove")> {
  public:
    WindowMouseMove(float x_, float y_) : x(x_), y(y_) {}

    float x, y;
};

} // namespace atta::event
