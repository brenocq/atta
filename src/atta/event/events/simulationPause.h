// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_EVENT_EVENTS_SIMULATION_PAUSE_H
#define ATTA_EVENT_EVENTS_SIMULATION_PAUSE_H
#include <atta/event/event.h>

namespace atta::event {

class SimulationPause : public EventTyped<SID("SimulationPause")> {
  public:
    SimulationPause() = default;
};

} // namespace atta::event

#endif // ATTA_EVENT_EVENTS_SIMULATION_PAUSE_H
