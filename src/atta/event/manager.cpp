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

void Manager::subscribeImpl(Event::Type type, Callback&& callback) { _observers[type].push_back(callback); }

void Manager::publishImpl(Event& event) const {
    Event::Type type = event.getType();

    // If there are no observers
    if (_observers.find(type) == _observers.end())
        return;

    // Loop over observers until event is handled
    for (auto& observer : _observers.at(type)) {
        observer(event);
        if (event.handled)
            return;
    }
}

void Manager::clearImpl() { _observers.clear(); }

} // namespace atta::event
