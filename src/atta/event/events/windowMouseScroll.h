// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class WindowMouseScroll : public EventTyped<SID("WindowMouseScroll")> {
  public:
    WindowMouseScroll(float dx_, float dy_) : dx(dx_), dy(dy_) {}

    float dx, dy;
};

} // namespace atta::event
