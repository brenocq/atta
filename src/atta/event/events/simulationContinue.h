//--------------------------------------------------
// Atta Event Module
// simulationContinue.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_SIMULATION_CONTINUE_H
#define ATTA_EVENT_EVENTS_SIMULATION_CONTINUE_H
#include <atta/event/event.h>

namespace atta::event {

class SimulationContinue : public EventTyped<SID("SimulationContinue")> {
  public:
    SimulationContinue() = default;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_CONTINUE_H
