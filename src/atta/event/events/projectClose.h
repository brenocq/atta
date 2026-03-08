// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_PROJECT_CLOSE_H
#define ATTA_EVENT_EVENTS_PROJECT_CLOSE_H
#include <atta/event/event.h>

namespace atta::event {

class ProjectClose : public EventTyped<SID("ProjectClose")> {
  public:
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_PROJECT_CLOSE_H
