// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_CREATE_ENTITY_H
#define ATTA_EVENT_EVENTS_CREATE_ENTITY_H
#include <atta/component/interface.h>
#include <atta/event/event.h>

namespace atta::event {

class CreateEntity : public EventTyped<SID("CreateEntity")> {
  public:
    component::EntityId entityId;
};

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_CREATE_ENTITY_H
