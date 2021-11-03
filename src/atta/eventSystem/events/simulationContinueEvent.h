//--------------------------------------------------
// Atta Event System
// simulationContinueEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_CONTINUE_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_CONTINUE_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class SimulationContinueEvent : public EventTyped<SID("SimulationContinueEvent")>
    {
    public:
        SimulationContinueEvent() = default;
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_CONTINUE_EVENT_H
