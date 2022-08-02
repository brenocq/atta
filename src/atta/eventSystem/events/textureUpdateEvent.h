//--------------------------------------------------
// Atta Event System
// textureUpdateEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_TEXTURE_UPDATE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_TEXTURE_UPDATE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class TextureUpdateEvent : public EventTyped<SID("TextureUpdateEvent")>
    {
    public:
        TextureUpdateEvent(StringId sid_): sid(sid_) {}

        const StringId sid;
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_TEXTURE_UPDATE_EVENT_H
