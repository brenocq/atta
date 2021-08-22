//--------------------------------------------------
// Atta Event System
// eventDispatcher.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/eventSystem/eventDispatcher.h>

namespace atta
{
	void EventDispatcher::subscribe(Event::Type type, Callback&& callback)
	{
		_observers[type].push_back(callback);
	}

	void EventDispatcher::publish(Event& event) const
	{
		Event::Type type = event.getType();

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
