// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/physics/engines/engine.h>

namespace atta::physics {

class NoneEngine : public Engine {
  public:
    NoneEngine();
    ~NoneEngine();

    void start() override;
    void step(float dt) override;
    void stop() override;
};

} // namespace atta::physics
