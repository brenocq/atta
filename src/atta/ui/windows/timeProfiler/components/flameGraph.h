// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::ui {

class FlameGraph {
  public:
    void render();

  private:
    void compute();
    void reset();

    struct FlameBar {
        StringId name;
        Profiler::Time time;
        std::vector<FlameBar> nested;
    };
    FlameBar _flameBar;
    size_t _lastRecordsSize;
};

} // namespace atta::ui
