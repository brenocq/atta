//--------------------------------------------------
// Atta Event Module
// event.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENT_H
#define ATTA_EVENT_EVENT_H
#include <atta/utils/stringId.h>

namespace atta::event {

class Event {
  public:
    using Type = StringHash;

    Event(Event::Type type) : _type(type) {}
    virtual ~Event() = default;

    Event::Type getType() const { return _type; }
    std::string getName() const { return StringId(_type).getString(); }

    bool handled = false;

  private:
    Event::Type _type;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.getName(); }

// TODO Template char* argument to save the string name when attributing the static member
template <Event::Type type_>
class EventTyped : public Event {
  public:
    EventTyped() : Event(type_) {}

    static const Event::Type type = type_;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENT_H
