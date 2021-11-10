//--------------------------------------------------
// Atta Event System
// scriptTargetEvent.h
// Date: 2021-11-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_SCRIPT_TARGET_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_SCRIPT_TARGET_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class ScriptTargetEvent : public EventTyped<SID("ScriptTargetEvent")>
    {
    public:
        std::vector<StringId> scriptSids;
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_SCRIPT_TARGET_EVENT_H
