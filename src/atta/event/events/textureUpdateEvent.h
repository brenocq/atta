//--------------------------------------------------
// Atta Event Module
// textureUpdateEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_TEXTURE_UPDATE_EVENT_H
#define ATTA_EVENT_EVENTS_TEXTURE_UPDATE_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class TextureUpdateEvent : public EventTyped<SID("TextureUpdateEvent")> {
  public:
    TextureUpdateEvent(StringId sid_) : sid(sid_) {}

    const StringId sid;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_TEXTURE_UPDATE_EVENT_H
