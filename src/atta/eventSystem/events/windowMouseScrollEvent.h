//--------------------------------------------------
// Atta Event System
// windowMouseScrollEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_SCROLL_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_SCROLL_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class WindowMouseScrollEvent : public Event
	{
	public:
		WindowMouseScrollEvent(float dx_, float dy_): dx(dx_), dy(dy_) {}

		Event::Type getType() const override { return SSID("Window_MouseScroll"); }
		const char* getName() const override { return "Window_MouseScroll"; }

		float dx, dy;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_SCROLL_H
