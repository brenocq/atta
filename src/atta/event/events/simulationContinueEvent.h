//--------------------------------------------------
// Atta Event Module
// simulationContinueEvent.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_SIMULATION_CONTINUE_EVENT_H
#define ATTA_EVENT_EVENTS_SIMULATION_CONTINUE_EVENT_H
#include <atta/event/event.h>

namespace atta::event {
class SimulationContinueEvent : public EventTyped<SID("SimulationContinueEvent")> {
  public:
    SimulationContinueEvent() = default;
};
} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_CONTINUE_EVENT_H
