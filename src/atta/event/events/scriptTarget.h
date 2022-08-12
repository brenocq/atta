//--------------------------------------------------
// Atta Event Module
// scriptTarget.h
// Date: 2021-11-06
// By Breno Cunha Queiroz
//--------------------------------------------------
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
