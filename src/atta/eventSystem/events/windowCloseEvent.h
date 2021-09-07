//--------------------------------------------------
// Atta Event System
// windowCloseEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_CLOSE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_CLOSE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class WindowCloseEvent : public Event
	{
	public:
		Event::Type getType() const override { return SSID("Window_Close"); }
		const char* getName() const override { return "Window_Close"; }
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_CLOSE_H
