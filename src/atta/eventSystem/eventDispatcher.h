//--------------------------------------------------
// Atta Event System
// eventDispatcher.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENT_DISPATCHER_H
#define ATTA_EVENT_SYSTEM_EVENT_DISPATCHER_H
#include <atta/eventSystem/event.h>

namespace atta
{
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

	class EventDispatcher final
	{
	public:
		using Callback = std::function<void(Event&)>;

		void subscribe(Event::Type type, Callback&& callback);
  		void publish(Event& event) const;

	private:
		std::unordered_map<Event::Type, std::vector<Callback>> _observers;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENT_DISPATCHER_H
