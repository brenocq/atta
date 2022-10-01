//--------------------------------------------------
// Atta Event Module
// simulationStep.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_SIMULATION_STEP_H
#define ATTA_EVENT_EVENTS_SIMULATION_STEP_H
#include <atta/event/event.h>

namespace atta::event {

class SimulationStep : public EventTyped<SID("SimulationStep")> {
  public:
    SimulationStep() = default;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_STEP_H
