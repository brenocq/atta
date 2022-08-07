//--------------------------------------------------
// Atta Event Module
// fileWatchEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_FILE_WATCH_EVENT_H
#define ATTA_EVENT_EVENTS_FILE_WATCH_EVENT_H
#include <atta/event/event.h>

namespace atta
{
    class FileWatchEvent : public EventTyped<SID("FileWatchEvent")>
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

        FileWatchEvent(): action(FileWatchEvent::NONE) {}

        Action action;
        fs::path file;
    };

    inline std::stringstream& operator<<(std::stringstream& ss, const FileWatchEvent& e)
    {
        ss << "FileWatchEvent{file=" << e.file << ", action={ ";
        if(e.action & FileWatchEvent::MODIFY)
            ss << "MODIFY ";
        if(e.action & FileWatchEvent::DELETE)
            ss << "DELETE ";
        if(e.action & FileWatchEvent::OPEN)
            ss << "OPEN ";
        if(e.action & FileWatchEvent::CLOSE)
            ss << "CLOSE ";
        if(e.action == FileWatchEvent::NONE)
            ss << "NONE ";
        ss << "}}";

        return ss;
    }
}
#endif// ATTA_EVENT_EVENTS_FILE_WATCH_EVENT_H
