//--------------------------------------------------
// Atta Event Module
// manager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENT_MANAGER_H
#define ATTA_EVENT_EVENT_MANAGER_H

namespace atta::event {

class Manager final {
  public:
    static Manager& getInstance();

    template <typename E>
    friend void subscribe(void* source, Callback&& callback);
    template <typename E>
    friend void unsubscribe(void* source, Callback&& callback);
    friend void publish(Event& event);
    friend void clear();

  private:
    void subscribeImpl(Event::Type type, void* source, Callback&& callback);
    void unsubscribeImpl(Event::Type type, void* source, Callback&& callback);

    void publishImpl(Event& event) const;
    void clearImpl();

    struct Observer {
        void* source;
        Callback callback;
    };
    std::unordered_map<Event::Type, std::vector<Observer>> _observers;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENT_MANAGER_H
