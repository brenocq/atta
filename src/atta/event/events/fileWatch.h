//--------------------------------------------------
// Atta Event Module
// fileWatch.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_FILE_WATCH_H
#define ATTA_EVENT_EVENTS_FILE_WATCH_H
#include <atta/event/event.h>

namespace atta::event {

class FileWatch : public EventTyped<SID("FileWatch")> {
  public:
    enum Action { NONE = 0, MODIFY = 1, DELETE = 2, OPEN = 4, CLOSE = 8 };

    FileWatch() : action(FileWatch::NONE) {}

    Action action;
    fs::path file;
};

inline std::stringstream& operator<<(std::stringstream& ss, const FileWatch& e) {
    ss << "FileWatch{file=" << e.file << ", action={ ";
    if (e.action & FileWatch::MODIFY)
        ss << "MODIFY ";
    if (e.action & FileWatch::DELETE)
        ss << "DELETE ";
    if (e.action & FileWatch::OPEN)
        ss << "OPEN ";
    if (e.action & FileWatch::CLOSE)
        ss << "CLOSE ";
    if (e.action == FileWatch::NONE)
        ss << "NONE ";
    ss << "}}";

    return ss;
}

} // namespace atta::event
#endif // ATTA_EVENT_EVENTS_FILE_WATCH_H
