//--------------------------------------------------
// Atta Event System
// simulationStartEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_START_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_START_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class SimulationStartEvent : public EventTyped<SID("SimulationStartEvent")>
    {
    public:
        SimulationStartEvent() = default;
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_START_EVENT_H
