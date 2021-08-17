//--------------------------------------------------
// Atta Event System
// dispatcher.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "dispatcher.h"

namespace atta
{

	void Dispatcher::subscribe(const EventType& type, Slot&& slot)
	{
		_observers[type].push_back(slot);
	}

	void Dispatcher::post(const Event& event) const
	{
		EventType type = event.getType();

		// If there are no observers
		if(_observers.find(type) == _observers.end())
			return;

		// Loop over observers until event is handled
		for(auto& observer : _observers.at(type))
		{
			observer(event);
			if(event.handled)
				return;
		}
	}

}
