//--------------------------------------------------
// Atta Event Module
// materialDestroy.h
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_MATERIAL_DESTROY_H
#define ATTA_EVENT_EVENTS_MATERIAL_DESTROY_H
#include <atta/event/event.h>

namespace atta::event {

class MaterialDestroy : public EventTyped<SID("MaterialDestroy")> {
  public:
    MaterialDestroy(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_MATERIAL_DESTROY_H
