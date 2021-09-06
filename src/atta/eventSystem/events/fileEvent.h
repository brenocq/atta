//--------------------------------------------------
// Atta Event System
// fileEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_FILE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_FILE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class FileEvent : public Event
	{
	public:
		enum class Action
		{
			NONE = 0,
			MODIFY = 1,
			DELETE = 2,
			OPEN = 4,
			CLOSE = 8
		};

		FileEvent(): action(Action::NONE) {}

		Event::Type getType() const override { return SID("File"); }
		const char* getName() const override { return "File"; }

		Action action;
		fs::path file;
	};

	inline FileEvent::Action operator|(FileEvent::Action a, FileEvent::Action b)
	{
		return static_cast<FileEvent::Action>(static_cast<int>(a) | static_cast<int>(b));
	}
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_FILE_EVENT_H
