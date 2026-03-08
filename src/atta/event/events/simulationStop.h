// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_SIMULATION_STOP_H
#define ATTA_EVENT_EVENTS_SIMULATION_STOP_H
#include <atta/event/event.h>

namespace atta::event {

class SimulationStop : public EventTyped<SID("SimulationStop")> {
  public:
    SimulationStop() = default;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_STOP_H
