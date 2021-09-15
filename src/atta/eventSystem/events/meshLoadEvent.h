//--------------------------------------------------
// Atta Event System
// meshLoadEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_MESH_LOAD_EVENT_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_MESH_LOAD_EVENT_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class MeshLoadEvent : public EventTyped<SID("MeshLoadEvent")>
	{
	public:
		MeshLoadEvent(StringId sid_): sid(sid_) {}

		const StringId sid;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_MESH_LOAD_EVENT_H
