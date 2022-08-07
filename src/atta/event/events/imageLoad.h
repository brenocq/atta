//--------------------------------------------------
// Atta Event Module
// imageLoad.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_IMAGE_LOAD_H
#define ATTA_EVENT_EVENTS_IMAGE_LOAD_H
#include <atta/event/event.h>

namespace atta::event {

class ImageLoad : public EventTyped<SID("ImageLoad")> {
  public:
    ImageLoad(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_IMAGE_LOAD_H
