//--------------------------------------------------
// Atta Event System
// dispatcher.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_DISPATCHER_H
#define ATTA_EVENT_SYSTEM_DISPATCHER_H
#include "event.h"

namespace atta
{

	class Dispatcher
	{
	public:
		using Slot = std::function<void(const Event&)>;

		void subscribe(const EventType& type, Slot&& slot);
  		void post(const Event& event) const;

	private:
		std::unordered_map<EventType, std::vector<Slot>> _observers;
	};

}

#endif// ATTA_EVENT_SYSTEM_DISPATCHER_H
