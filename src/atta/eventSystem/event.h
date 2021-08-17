//--------------------------------------------------
// Atta Event System
// event.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENT_H

namespace atta
{

	enum class EventType {
		NONE = 0,
		// Window
		WINDOW_MOUSE_MOVE,
		WINDOW_MOUSE_PRESS,
		WINDOW_MOUSE_RELEASE,
		WINDOW_MOUSE_SCROLL,
		WINDOW_KEY_PRESS,
		WINDOW_KEY_REPEAT,
		WINDOW_KEY_RELEASE,
		WINDOW_FOCUS,
		WINDOW_LOST_FOCUS,
		WINDOW_MOVE,
		WINDOW_CLOSE,
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType getType() const = 0;
		virtual const char* getName() const = 0;
		virtual std::string toString() const { return getName(); }

		bool handled;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

}

#endif// ATTA_EVENT_SYSTEM_EVENT_H
