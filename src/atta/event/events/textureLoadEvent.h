//--------------------------------------------------
// Atta Event Module
// textureLoadEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_TEXTURE_LOAD_EVENT_H
#define ATTA_EVENT_EVENTS_TEXTURE_LOAD_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class TextureLoadEvent : public EventTyped<SID("TextureLoadEvent")> {
  public:
    TextureLoadEvent(StringId sid_) : sid(sid_) {}

    const StringId sid;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_TEXTURE_LOAD_EVENT_H
