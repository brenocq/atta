//--------------------------------------------------
// Atta Event System
// dispatcher.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_DISPATCHER_H
#define ATTA_EVENT_SYSTEM_DISPATCHER_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class Dispatcher
	{
	public:
		using Callback = std::function<void(Event&)>;

		void subscribe(const EventType& type, Callback&& callback);
  		void publish(Event& event) const;

	private:
		std::unordered_map<EventType, std::vector<Callback>> _observers;
	};
}

#endif// ATTA_EVENT_SYSTEM_DISPATCHER_H
