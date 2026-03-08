// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_WINDOW_MOUSE_SCROLL_H
#define ATTA_EVENT_EVENTS_WINDOW_MOUSE_SCROLL_H
#include <atta/event/event.h>

namespace atta::event {

class WindowMouseScroll : public EventTyped<SID("WindowMouseScroll")> {
  public:
    WindowMouseScroll(float dx_, float dy_) : dx(dx_), dy(dy_) {}

    float dx, dy;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_MOUSE_SCROLL_H
