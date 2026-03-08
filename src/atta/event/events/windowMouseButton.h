// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class WindowMouseButton : public EventTyped<SID("WindowMouseButton")> {
  public:
    enum class Action { PRESS, RELEASE };

    WindowMouseButton(int button_, Action action_) : button(button_), action(action_) {}

    int button;
    Action action;
};

} // namespace atta::event
