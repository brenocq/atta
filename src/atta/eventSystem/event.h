//--------------------------------------------------
// Atta Event System
// event.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENT_H
#include <atta/core/stringId.h>

namespace atta
{
	class Event
	{
	public:
		virtual ~Event() = default;

		using Type = StringHash;

		virtual Type getType() const = 0;
		virtual const char* getName() const = 0;
		std::string toString() const { return getName(); }

		bool handled = false;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}

#endif// ATTA_EVENT_SYSTEM_EVENT_H
