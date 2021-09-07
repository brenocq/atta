//--------------------------------------------------
// Atta Event System
// windowMouseMoveEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_MOVE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_MOVE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class WindowMouseMoveEvent : public Event
	{
	public:
		WindowMouseMoveEvent(float x_, float y_): x(x_), y(y_) {}

		Event::Type getType() const override { return SSID("Window_MouseMove"); }
		const char* getName() const override { return "Window_MouseMove"; }

		float x, y;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_MOVE_H
