// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>

namespace atta::event {

class ImageLoad : public EventTyped<SID("ImageLoad")> {
  public:
    ImageLoad(StringId sid_) : sid(sid_) {}

    const StringId sid;
};

} // namespace atta::event
