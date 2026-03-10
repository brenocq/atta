// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class WindowFocus : public EventTyped<SID("WindowFocus")> {
  public:
    WindowFocus(bool hasFocus_) : hasFocus(hasFocus_) {}

    bool hasFocus;
};

} // namespace atta::event
