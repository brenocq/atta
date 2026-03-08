// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_DELETE_COMPONENT_H
#define ATTA_EVENT_EVENTS_DELETE_COMPONENT_H
#include <atta/component/interface.h>
#include <atta/event/event.h>

namespace atta::event {

class DeleteComponent : public EventTyped<SID("DeleteComponent")> {
  public:
    component::ComponentId componentId;
    component::EntityId entityId;
};

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_DELETE_COMPONENT_H
