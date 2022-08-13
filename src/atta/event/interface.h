//--------------------------------------------------
// Atta Event Module
// interface.h
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_INTERFACE_H
#define ATTA_EVENT_INTERFACE_H

#include <atta/event/event.h>

namespace atta::event {

using Callback = std::function<void(Event&)>;
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

// static void subscribe(Event::Type type, Callback&& callback) { getInstance().subscribeImpl(type, std::move(callback)); }
template <typename E>
void subscribe(Callback&& callback);
// static void subscribe(Event::Type type, Callback&& callback) { getInstance()._observers[type].push_back(callback); }
void publish(Event& event);
void clear();

} // namespace atta::event

#include <atta/event/manager.h>

namespace atta::event {

template <typename E>
void subscribe(Callback&& callback) {
    Manager::getInstance().subscribeImpl(E::type, std::move(callback));
}

} // namespace atta::event

#endif // ATTA_COMPONENT_INTERFACE_H
