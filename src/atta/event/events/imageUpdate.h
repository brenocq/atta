// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class ImageUpdate : public EventTyped<SID("ImageUpdate")> {
  public:
    ImageUpdate(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event
