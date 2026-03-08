// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class SimulationContinue : public EventTyped<SID("SimulationContinue")> {
  public:
    SimulationContinue() = default;
};

} // namespace atta::event
