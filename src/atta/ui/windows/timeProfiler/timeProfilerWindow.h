// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/ui/windows/timeProfiler/components/flameGraph.h>
#include <atta/ui/windows/timeProfiler/components/tearDown.h>
#include <atta/ui/windows/timeProfiler/components/timeline.h>
#include <atta/ui/windows/window.h>

namespace atta::ui {

class TimeProfilerWindow : public Window<TimeProfilerWindow> {
  private:
    TimeProfilerWindow();
    void renderImpl();

    friend Window<TimeProfilerWindow>;

  private:
    TearDown _tearDown;
    FlameGraph _flameGraph;
    Timeline _timeline;
};

} // namespace atta::ui
