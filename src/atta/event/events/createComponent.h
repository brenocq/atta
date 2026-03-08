// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/component/interface.h>
#include <atta/event/event.h>

namespace atta::event {

class CreateComponent : public EventTyped<SID("CreateComponent")> {
  public:
    component::ComponentId componentId;
    component::EntityId entityId;
    component::Component* component;
};

} // namespace atta::event
