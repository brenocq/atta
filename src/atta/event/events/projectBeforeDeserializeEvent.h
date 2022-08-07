//--------------------------------------------------
// Atta Event Module
// projectBeforeDeserializeEvent.h
// Date: 2021-11-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_PROJECT_BEFORE_DESERIALIZE_EVENT_H
#define ATTA_EVENT_EVENTS_PROJECT_BEFORE_DESERIALIZE_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class ProjectBeforeDeserializeEvent : public EventTyped<SID("ProjectBeforeDeserializeEvent")> {
  public:
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_PROJECT_BEFORE_DESERIALIZE_EVENT_H
