// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/io/interface.h>

namespace atta::io {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().startUpImpl(); }
void update() { Manager::getInstance().startUpImpl(); }

} // namespace atta::io
