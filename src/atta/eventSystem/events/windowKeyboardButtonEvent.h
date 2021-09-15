//--------------------------------------------------
// Atta Event System
// windowKeyboardButtonEvent.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_WINDOW_KEYBOARD_BUTTON_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_WINDOW_KEYBOARD_BUTTON_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class WindowKeyboardButtonEvent : public EventTyped<SID("WindowKeyboardButtonEvent")>
	{
	public:
		enum class Action
		{
			PRESS,
			REPEAT,
			RELEASE
		};

		WindowKeyboardButtonEvent(int key_, Action action_): 
			key(key_), action(action_) {}

		int key;
		Action action;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_WINDOW_KEYBOARD_BUTTON_H
