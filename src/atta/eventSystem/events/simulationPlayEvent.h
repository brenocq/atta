//--------------------------------------------------
// Atta Event System
// simulationPlayEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_PLAY_EVENT_H
#define ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_PLAY_EVENT_H
#include <atta/eventSystem/event.h>

namespace atta
{
    class SimulationPlayEvent : public EventTyped<SID("SimulationPlayEvent")>
    {
    public:
        SimulationPlayEvent() = default;
    };
}

#endif// ATTA_EVENT_SYSTEM_EVENTS_SIMULATION_PLAY_EVENT_H
