//--------------------------------------------------
// Atta Event Module
// projectSave.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_PROJECT_SAVE_H
#define ATTA_EVENT_EVENTS_PROJECT_SAVE_H
#include <atta/event/event.h>

namespace atta::event {

class ProjectSave : public EventTyped<SID("ProjectSave")> {
  public:
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_PROJECT_SAVE_H
