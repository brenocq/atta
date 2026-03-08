// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
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
