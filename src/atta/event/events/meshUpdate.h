//--------------------------------------------------
// Atta Event Module
// meshUpdate.h
// Date: 2023-12-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_MESH_UPDATE_H
#define ATTA_EVENT_EVENTS_MESH_UPDATE_H
#include <atta/event/event.h>

namespace atta::event {

class MeshUpdate : public EventTyped<SID("MeshUpdate")> {
  public:
    MeshUpdate(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_MESH_UPDATE_H
