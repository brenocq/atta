//--------------------------------------------------
// Atta Event Module
// manager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENT_MANAGER_H
#define ATTA_EVENT_EVENT_MANAGER_H
#include <atta/event/event.h>

namespace atta::event {

#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

class Manager final {
  public:
    static Manager& getInstance();

    using Callback = std::function<void(Event&)>;

    // static void subscribe(Event::Type type, Callback&& callback) { getInstance().subscribeImpl(type, std::move(callback)); }
    template <typename E>
    static void subscribe(Callback&& callback) {
        getInstance().subscribeImpl(E::type, std::move(callback));
    }
    // static void subscribe(Event::Type type, Callback&& callback) { getInstance()._observers[type].push_back(callback); }
    static void publish(Event& event) { getInstance().publishImpl(event); }
    static void clear() { getInstance().clearImpl(); }

  private:
    void subscribeImpl(Event::Type type, Callback&& callback);

    void publishImpl(Event& event) const;
    void clearImpl();

    std::unordered_map<Event::Type, std::vector<Callback>> _observers;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENT_MANAGER_H
