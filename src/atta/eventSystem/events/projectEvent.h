//--------------------------------------------------
// Atta Event System
// project.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_PROJECT_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_PROJECT_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class ProjectEvent : public Event
	{
	public:
		Event::Type getType() const override { return SID("Project"); }
		const char* getName() const override { return "Project"; }
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_PROJECT_H
