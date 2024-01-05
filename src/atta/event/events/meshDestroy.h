//--------------------------------------------------
// Atta Event Module
// meshDestroy.h
// Date: 2024-01-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_MESH_DESTROY_H
#define ATTA_EVENT_EVENTS_MESH_DESTROY_H
#include <atta/event/event.h>

namespace atta::event {

class MeshDestroy : public EventTyped<SID("MeshDestroy")> {
  public:
    MeshDestroy(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_MESH_DESTROY_H
