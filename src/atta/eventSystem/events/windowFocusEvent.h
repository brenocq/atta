//--------------------------------------------------
// Atta Event System
// windowFocusEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_FOCUS_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_FOCUS_EVENT_H
#include <atta/eventSystem/event.h>
#include <atta/core/window.h>

namespace atta
{
	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(bool hasFocus_): hasFocus(hasFocus_) {}

		Event::Type getType() const { return static_cast<Event::Type>(Window::Event::FOCUS); }
		const char* getName() const { return "Window_Focus"; }

		bool hasFocus;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_FOCUS_H
