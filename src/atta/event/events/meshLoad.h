//--------------------------------------------------
// Atta Event Module
// meshLoad.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_MESH_LOAD_H
#define ATTA_EVENT_EVENTS_MESH_LOAD_H
#include <atta/event/event.h>

namespace atta::event {

class MeshLoad : public EventTyped<SID("MeshLoad")> {
  public:
    MeshLoad(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_MESH_LOAD_H
