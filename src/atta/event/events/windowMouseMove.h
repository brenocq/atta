//--------------------------------------------------
// Atta Event Module
// windowMouseMove.h
// Date: 2021-08-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_WINDOW_MOUSE_MOVE_H
#define ATTA_EVENT_EVENTS_WINDOW_MOUSE_MOVE_H
#include <atta/event/event.h>

namespace atta::event {

class WindowMouseMove : public EventTyped<SID("WindowMouseMove")> {
  public:
    WindowMouseMove(float x_, float y_) : x(x_), y(y_) {}

    float x, y;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_WINDOW_MOUSE_MOVE_H
