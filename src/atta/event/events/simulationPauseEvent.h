//--------------------------------------------------
// Atta Event Module
// simulationPauseEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_SIMULATION_PAUSE_EVENT_H
#define ATTA_EVENT_EVENTS_SIMULATION_PAUSE_EVENT_H
#include <atta/event/event.h>

namespace atta
{
    class SimulationPauseEvent : public EventTyped<SID("SimulationPauseEvent")>
    {
    public:
        SimulationPauseEvent() = default;
    };
}

#endif// ATTA_EVENT_EVENTS_SIMULATION_PAUSE_EVENT_H
