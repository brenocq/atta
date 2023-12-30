//--------------------------------------------------
// Atta Event Module
// materialUpdate.h
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_MATERIAL_UPDATE_H
#define ATTA_EVENT_EVENTS_MATERIAL_UPDATE_H
#include <atta/event/event.h>

namespace atta::event {

class MaterialUpdate : public EventTyped<SID("MaterialUpdate")> {
  public:
    MaterialUpdate(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_MATERIAL_UPDATE_H
