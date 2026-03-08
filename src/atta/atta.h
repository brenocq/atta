// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/event/event.h>
#include <atta/memory/allocators/stackAllocator.h>
#include <ctime>

namespace atta {
class Atta {
  public:
    struct CreateInfo {
        std::filesystem::path projectFile = "";
    };

    Atta(const CreateInfo& info);
    ~Atta();

    void run();
    void loop();
    void step();

  private:
    // Handle events
    void onWindowClose(event::Event& event);
    void onSimulationStateChange(event::Event& event);

    // Memory
    memory::StackAllocator* _mainAllocator;

    // State
    bool _shouldFinish;
    bool _shouldStep;
    clock_t _lastStep;
    clock_t _currStep;
};
} // namespace atta
