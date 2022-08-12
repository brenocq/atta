//--------------------------------------------------
// Atta Event Module
// simulationStart.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EVENT_EVENTS_SIMULATION_START_H
#define ATTA_EVENT_EVENTS_SIMULATION_START_H
#include <atta/event/event.h>

namespace atta::event {

class SimulationStart : public EventTyped<SID("SimulationStart")> {
  public:
    SimulationStart() = default;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_START_H
