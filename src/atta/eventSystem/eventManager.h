//--------------------------------------------------
// Atta Event System
// eventManager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENT_MANAGER_H
#define ATTA_EVENT_SYSTEM_EVENT_MANAGER_H
#include <atta/eventSystem/event.h>

namespace atta
{
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	class EventManager final
	{
	public:
		static EventManager& getInstance() {
			static EventManager instance;
			return instance;
		}

		using Callback = std::function<void(Event&)>;

		static void subscribe(Event::Type type, Callback&& callback) { getInstance().subscribeImpl(type, std::move(callback)); }
  		static void publish(Event& event) { getInstance().publishImpl(event); }

	private:
		void subscribeImpl(Event::Type type, Callback&& callback);
  		void publishImpl(Event& event) const;

		std::unordered_map<Event::Type, std::vector<Callback>> _observers;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENT_MANAGER_H
