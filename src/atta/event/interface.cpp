// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/event/interface.h>

namespace atta::event {

void publish(Event& event) { Manager::getInstance().publishImpl(event); }
void clear() { Manager::getInstance().clearImpl(); }

} // namespace atta::event
