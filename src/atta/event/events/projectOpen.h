// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_PROJECT_OPEN_H
#define ATTA_EVENT_EVENTS_PROJECT_OPEN_H
#include <atta/event/event.h>

namespace atta::event {

class ProjectOpen : public EventTyped<SID("ProjectOpen")> {
  public:
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_PROJECT_OPEN_H
