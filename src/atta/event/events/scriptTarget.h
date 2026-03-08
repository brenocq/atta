// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_SCRIPT_TARGET_H
#define ATTA_EVENT_EVENTS_SCRIPT_TARGET_H

#include <atta/event/event.h>

namespace atta::event {

class ScriptTarget : public EventTyped<SID("ScriptTarget")> {
  public:
    std::vector<StringId> scriptSids;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SCRIPT_TARGET_H
