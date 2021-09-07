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
		enum Action
		{
			NONE = 0,
			MODIFY = 1,
			DELETE = 2,
			OPEN = 4,
			CLOSE = 8
		};

		FileEvent(): action(FileEvent::NONE) {}

		Event::Type getType() const override { return SSID("File"); }
		const char* getName() const override { return "File"; }

		Action action;
		fs::path file;
	};

	inline std::stringstream& operator<<(std::stringstream& ss, const FileEvent& e)
	{
		ss << "FileEvent{file=" << e.file << ", action={ ";
		if(e.action & FileEvent::MODIFY)
			ss << "MODIFY ";
		if(e.action & FileEvent::DELETE)
			ss << "DELETE ";
		if(e.action & FileEvent::OPEN)
			ss << "OPEN ";
		if(e.action & FileEvent::CLOSE)
			ss << "CLOSE ";
		if(e.action == FileEvent::NONE)
			ss << "NONE ";
		ss << "}}";

		return ss;
	}

}
#endif// ATTA_EVENT_SYSTEM_EVENTS_FILE_EVENT_H
