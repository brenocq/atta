//--------------------------------------------------
// Atta Event System
// simulationStopEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_STOP_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_STOP_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class SimulationStopEvent : public EventTyped<SID("SimulationStopEvent")>
	{
	public:
		SimulationStopEvent() = default;
	};
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_STOP_EVENT_H
