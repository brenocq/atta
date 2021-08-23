//--------------------------------------------------
// Atta Event System
// windowResizeEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_RESIZE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_RESIZE_EVENT_H
#include <atta/eventSystem/event.h>
#include <atta/core/window.h>

namespace atta
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(size_t width_, size_t height_): width(width_), height(height_) {}

		Event::Type getType() const override { return static_cast<Event::Type>(Window::Event::RESIZE); }
		const char* getName() const override { return "Window_Resize"; }

		size_t width, height;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_RESIZE_H
