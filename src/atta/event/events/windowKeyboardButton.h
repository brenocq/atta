// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class WindowKeyboardButton : public EventTyped<SID("WindowKeyboardButton")> {
  public:
    enum class Action { PRESS, REPEAT, RELEASE };

    WindowKeyboardButton(int key_, Action action_) : key(key_), action(action_) {}

    int key;
    Action action;
};

} // namespace atta::event
