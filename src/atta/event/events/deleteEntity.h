// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/component/interface.h>
#include <atta/event/event.h>

namespace atta::event {

class DeleteEntity : public EventTyped<SID("DeleteEntity")> {
  public:
    component::EntityId entityId;
};

} // namespace atta::event
