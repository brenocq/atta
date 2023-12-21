//--------------------------------------------------
// Atta Event Module
// materialCreate.h
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_MATERIAL_CREATE_H
#define ATTA_EVENT_EVENTS_MATERIAL_CREATE_H
#include <atta/event/event.h>

namespace atta::event {

class MaterialCreate : public EventTyped<SID("MaterialCreate")> {
  public:
    MaterialCreate(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_MATERIAL_CREATE_H
