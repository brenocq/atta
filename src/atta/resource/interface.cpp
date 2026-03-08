// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/resource/interface.h>

namespace atta::resource {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }
void update() { Manager::getInstance().updateImpl(); }

} // namespace atta::resource
