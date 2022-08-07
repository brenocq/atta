//--------------------------------------------------
// Atta Event Module
// simulationStartEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_SIMULATION_START_EVENT_H
#define ATTA_EVENT_EVENTS_SIMULATION_START_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class SimulationStartEvent : public EventTyped<SID("SimulationStartEvent")> {
  public:
    SimulationStartEvent() = default;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_START_EVENT_H
