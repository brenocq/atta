//--------------------------------------------------
// Atta Event Module
// simulationStopEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_SIMULATION_STOP_EVENT_H
#define ATTA_EVENT_EVENTS_SIMULATION_STOP_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class SimulationStopEvent : public EventTyped<SID("SimulationStopEvent")> {
  public:
    SimulationStopEvent() = default;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_STOP_EVENT_H
