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
    friend void subscribe(Callback&& callback);
    friend void publish(Event& event);
    friend void clear();

  private:
    void subscribeImpl(Event::Type type, Callback&& callback);

    void publishImpl(Event& event) const;
    void clearImpl();

    std::unordered_map<Event::Type, std::vector<Callback>> _observers;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENT_MANAGER_H
