//--------------------------------------------------
// Atta Event Module
// materialLoad.h
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_MATERIAL_LOAD_H
#define ATTA_EVENT_EVENTS_MATERIAL_LOAD_H
#include <atta/event/event.h>

namespace atta::event {

class MaterialLoad : public EventTyped<SID("MaterialLoad")> {
  public:
    MaterialLoad(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_MATERIAL_LOAD_H
