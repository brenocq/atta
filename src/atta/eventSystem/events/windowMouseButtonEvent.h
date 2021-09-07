//--------------------------------------------------
// Atta Event System
// windowMouseButtonEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_BUTTON_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_BUTTON_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class WindowMouseButtonEvent : public Event
	{
	public:
		enum class Action
		{
			PRESS,
			RELEASE
		};

		WindowMouseButtonEvent(int button_, Action action_): button(button_), action(action_) {}

		Event::Type getType() const override { return SSID("Window_MouseButton"); }
		const char* getName() const override { return "Window_MouseButton"; }

		int button;
		Action action;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_MOUSE_BUTTON_H
