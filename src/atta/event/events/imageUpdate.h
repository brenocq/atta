//--------------------------------------------------
// Atta Event Module
// imageUpdate.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_IMAGE_UPDATE_H
#define ATTA_EVENT_EVENTS_IMAGE_UPDATE_H
#include <atta/event/event.h>

namespace atta::event {

class ImageUpdate : public EventTyped<SID("ImageUpdate")> {
  public:
    ImageUpdate(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_IMAGE_UPDATE_H
