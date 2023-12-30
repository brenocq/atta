//--------------------------------------------------
// Atta Event Module
// manager.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/event/interface.h>
#include <atta/event/manager.h>

namespace atta::event {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::subscribeImpl(Event::Type type, void* source, Callback&& callback) {
    // Make sure source has only one observer for this type
    for (size_t i = 0; i < _observers[type].size(); i++) {
        if (_observers[type][i].source == source) {
            _observers[type].erase(_observers[type].begin() + i);
            LOG_WARN("evt::Manager", "An object must not subscribe to the same event more than once");
            break;
        }
    }

    _observers[type].push_back({source, callback});
}

void Manager::unsubscribeImpl(Event::Type type, void* source, Callback&& callback) {
    // If there are no observers for this type
    if (_observers.find(type) == _observers.end())
        return;

    for (size_t i = 0; i < _observers[type].size(); i++) {
        if (_observers[type][i].source == source) {
            _observers[type].erase(_observers[type].begin() + i);
            break;
        }
    }
}

void Manager::publishImpl(Event& event) const {
    Event::Type type = event.getType();

    // If there are no observers
    if (_observers.find(type) == _observers.end())
        return;

    // Loop over observers until event is handled
    for (auto& observer : _observers.at(type)) {
        observer.callback(event);
        if (event.handled)
            return;
    }
}

void Manager::clearImpl() { _observers.clear(); }

} // namespace atta::event
