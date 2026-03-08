// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
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
